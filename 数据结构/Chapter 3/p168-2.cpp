// use c++20
#include <array>
#include <compare>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <random>
#include <ranges>
#include <variant>
using namespace std;
namespace rng = ranges;
// global variables
size_t id_in = 0, id_out = 1;
size_t current_time           = 0;
size_t emergency_landing_cnt  = 0;
size_t takeoff_n_per_unittime = 0;
size_t takeoff_wait_time_sum  = 0;
size_t landing_n_per_unittime = 0;
size_t landing_wait_time_sum  = 0;
size_t landing_left_time_sum  = 0;
size_t get_takeoff_id() { return exchange(id_out, id_out + 2); }
size_t get_landing_id() { return exchange(id_in, id_in + 2); }

// helper functions
template <class Adapter>
auto& get_container(Adapter& container) {
    struct hack : Adapter {
        static auto& get(Adapter& con) { return con.*&hack::c; }
    };
    return hack::get(container);
}

// data types
struct Plane {
    Plane(size_t id) : id(id) {}
    size_t id        = 0;
    size_t wait_time = 0;

    friend ostream& operator<<(ostream& os, const Plane& p) { return os << p.id; }
};

struct LandingPlane : public Plane {

    LandingPlane(size_t id, size_t left_time) : Plane(id), left_time(left_time) {}

    size_t left_time = 0;

    strong_ordering operator<=>(const LandingPlane& plane) const { return left_time <=> plane.left_time; }
};
using TakeoffPlane = Plane;
class RunwayBase {
    friend class Airport;

public:
    struct MaintainGuard {
        template <class... Args>
        MaintainGuard(Args&... arg) : MaintainGuard(nullptr, arg...) {}
        template <class... Args>
        MaintainGuard(RunwayBase* rw, Args&... arg) : rw(rw) {
            init(arg...);
        }
        ~MaintainGuard() {
            for (auto& q : queues) {
                [&]<size_t... Is>(index_sequence<Is...>) {
                    auto impl = [&]<size_t I>() {
                        if (auto** ptr = get_if<I>(addressof(q)); ptr) {
                            auto& c = get_container(**ptr);
                            if constexpr (is_same_v<remove_cvref_t<decltype(c)>::value_type, LandingPlane>) {
                                const auto res = rng::remove_if(c, [&](auto& plane) {
                                    plane.wait_time++;
                                    --plane.left_time;
                                    if (plane.left_time < 5)
                                        rw->handler(plane);
                                    return plane.left_time < 5;
                                });
                                c.erase(res.begin(), res.end());
                            }
                            else
                                rng::for_each(c, [](auto& plane) { plane.wait_time++; });
                        }
                    };
                    (impl.template operator()<Is>(), ...);
                }
                (make_index_sequence<variant_size_v<decltype(queues)::value_type>>());
            }
        }

    private:
        template <class Queue, class... Args>
        void init(Queue& q, Args&... arg) {
            queues.push_back(addressof(q));
            if constexpr (sizeof...(arg))
                init(arg...);
        }
        // clang-format off
        vector<variant<
            queue<LandingPlane>*, 
            queue<TakeoffPlane>*, 
            priority_queue<LandingPlane, vector<LandingPlane>, greater<>>*>> queues;
        // clang-format on
        RunwayBase* rw;
    };

    size_t takeoffSize() const { return takeoff.size(); }

    virtual void run(size_t* landing_cnt = nullptr) {
        MaintainGuard gaurd(this, takeoff, landing);
        if (!landing.empty())
            set_landing();
        else if (!takeoff.empty())
            set_takeoff();
    }
    virtual bool empty() const { return takeoff.empty(); }

    virtual bool dispatch(const LandingPlane& plane) {
        if (!landing.empty() && landing.top().left_time == 1)
            return false;
        LandingPlane p = plane;
        if (plane.id == 0)
            p.id = get_landing_id();
        landing.push(p);
        return true;
    }
    void dispatch(const TakeoffPlane& plane) {
        TakeoffPlane p = plane;
        p.id           = get_takeoff_id();
        takeoff.push(p);
    }

    unique_ptr<Plane> getCurrent() { return move(current); }

    template <class Fn>
    void set_emergency_handler(Fn h) {
        handler = h;
    }

protected:
    void set_landing() {
        current = make_unique<LandingPlane>(landing.top());
        landing_n_per_unittime++;
        landing_wait_time_sum += current->wait_time;
        landing_left_time_sum += static_cast<LandingPlane*>(current.get())->left_time;
        landing.pop();
    }

    void set_takeoff() {
        current = make_unique<TakeoffPlane>(move(takeoff.front()));
        takeoff_n_per_unittime++;
        takeoff_wait_time_sum += current->wait_time;
        takeoff.pop();
    }

    queue<TakeoffPlane>                                           takeoff;
    priority_queue<LandingPlane, vector<LandingPlane>, greater<>> landing;
    unique_ptr<Plane>                                             current = nullptr;

    function<void(const LandingPlane&)> handler;
};
class Runway : public RunwayBase {
    using RunwayBase::current;
    using RunwayBase::landing;
    using RunwayBase::set_landing;
    using RunwayBase::set_takeoff;
    using RunwayBase::takeoff;
    friend class Airport;

public:
    size_t landingSize() const { return wait1.size() + wait2.size(); }

    void addLandingPlane(const LandingPlane& plane) {
        if (wait1.size() < wait2.size())
            wait1.push(plane);
        else
            wait2.push(plane);
    }
    bool dispatch(const LandingPlane& plane) override {
        if (!landing.empty() && landing.top().left_time < plane.left_time)
            return false;
        LandingPlane p = plane;
        if (plane.id == 0)
            p.id = get_landing_id();
        landing.push(p);
        return true;
    }

    bool empty() const override { return takeoff.empty() && landingSize() == 0; }
    void run(size_t* landing_cnt) override {
        MaintainGuard guard(this, wait1, wait2, takeoff, landing);

        // wait queue --> landing queue
        if (auto& wait_queue = wait1.size() > wait2.size() ? wait1 : wait2; *landing_cnt < 3 && !wait_queue.empty()) {
            dispatch(wait_queue.front());
            wait_queue.pop();
            ++*landing_cnt;
        }

        if (!landing.empty() && landing.top().left_time == 1) {
            set_landing();
            return;
        }
        if (!takeoff.empty() && !landing.empty()) {
            if (takeoff.front().id < landing.top().id)
                set_takeoff();
            else
                set_landing();
        }
        else if (!takeoff.empty())
            set_takeoff();
        else if (!landing.empty())
            set_landing();
    }

private:
    queue<LandingPlane> wait1, wait2;
};
using EmergencyRunway = RunwayBase;

class Airport {
public:
    Airport() : queue_info("queue_info.csv", ios::trunc | ios::out), stuff_info("stuff_info.csv", ios::trunc | ios::out) {
        if (!queue_info || !stuff_info) {
            cerr << "fail to open file" << endl;
            exit(0);
        }
        queue_info << "time,"
                   << "size of wait1 of runway1, size of wait2 of runway1, takeoff queue of runway1,landing queue of runway1,"
                   << "size of wait1 of runway2, size of wait2 of runway2, takeoff queue of runway2,landing queue of runway2,"
                   << "takeoff queue of emergency runway,landing queue of emergency runway" << endl;
        stuff_info << "time,"
                   << "average of takeoff waiting time, average of landing waiting time,average of landing left time,"
                   << "number of emergency landing plane" << endl;

        arr[0] = make_unique<Runway>();
        arr[1] = make_unique<Runway>();
        arr[2] = make_unique<EmergencyRunway>();
        rng::for_each(arr, [&](auto& rw) { rw->set_emergency_handler([&](const LandingPlane& plane) { emergency_planes.push(plane); }); });
    }
    void addPlane(const TakeoffPlane& plane) { takeoff_planes.push(plane); }
    void addPlane(const LandingPlane& plane) {
        Runway& runway = runway1().landingSize() < runway2().landingSize() ? runway1() : runway2();
        runway.addLandingPlane(plane);
    }

    array<unique_ptr<Plane>, 3> run() {
        RunwayBase::MaintainGuard guard(takeoff_planes);

        size_t emergency_landing_cnt  = 0;
        size_t takeoff_n_per_unittime = 0;
        size_t takeoff_wait_time_sum  = 0;
        size_t landing_n_per_unittime = 0;
        size_t landing_wait_time_sum  = 0;
        size_t landing_left_time_sum  = 0;

        size_t landing_cnt = 0;  // [0, 3] per unit time

        // emergency queue --> landing queue

        if (!emergency_planes.empty()) {
            bool full;
            do {
                full = false;
                for (int i = arr.size() - 1; i >= 0; --i)
                    full |= push_emergency_plane(*arr[i], landing_cnt);
            } while (landing_cnt < 3 && !full);
        }

        emergency_landing_cnt = landing_cnt;

        for (size_t takeoff_cnt : views::iota(0, 3))
            push_takeoff_plane([&]() -> RunwayBase& {
                size_t min = 0;
                for (auto i : views::iota(1, 3))
                    if (arr[min]->takeoffSize() > arr[i]->takeoffSize())
                        min = i;
                return *arr[min];
            }());

        rng::for_each(arr, [&](auto& rw) { rw->run(&landing_cnt); });

        output_queue_info();
        output_stuff_info();

        return { move(runway1().getCurrent()), move(runway2().getCurrent()), move(emergency().getCurrent()) };
    }

    bool empty() const {
        for (auto& rw : arr) {
            if (!rw->empty())
                return false;
        }
        return takeoff_planes.empty() && emergency_planes.empty();
    }

    Runway&          runway1() { return static_cast<Runway&>(*arr[0]); }
    Runway&          runway2() { return static_cast<Runway&>(*arr[1]); }
    EmergencyRunway& emergency() { return static_cast<EmergencyRunway&>(*arr[2]); }

    ~Airport() {
        queue_info.close();
        stuff_info.close();
    }

private:
    void output_queue_info() {
        queue_info << format("{},{},{},", current_time, runway1().wait1.size(), runway1().wait2.size());
        rng::copy(get_container(runway1().takeoff), ostream_iterator<TakeoffPlane>(queue_info, " "));
        queue_info << ",";
        rng::copy(get_container(runway1().landing), ostream_iterator<LandingPlane>(queue_info, " "));
        queue_info << ",";
        queue_info << format("{},{},", runway2().wait1.size(), runway2().wait2.size());
        rng::copy(get_container(runway2().takeoff), ostream_iterator<TakeoffPlane>(queue_info, " "));
        queue_info << ",";
        rng::copy(get_container(runway2().landing), ostream_iterator<LandingPlane>(queue_info, " "));
        queue_info << ",";
        rng::copy(get_container(emergency().takeoff), ostream_iterator<TakeoffPlane>(queue_info, " "));
        queue_info << ",";
        rng::copy(get_container(emergency().landing), ostream_iterator<LandingPlane>(queue_info, " "));
        queue_info << endl;
    }

    void output_stuff_info() {
        // clang-format off
        stuff_info << format("{},{},{},{},{}\n", 
            current_time, 
            static_cast<double>(takeoff_wait_time_sum) / takeoff_n_per_unittime,
            static_cast<double>(landing_wait_time_sum) / landing_n_per_unittime,
            static_cast<double>(landing_left_time_sum) / landing_n_per_unittime,
            emergency_landing_cnt
            );
        // clang-format on
    }
    void push_takeoff_plane(RunwayBase& rw) {
        if (!takeoff_planes.empty()) {  // takeoff-wait queue --> takeoff queue
            rw.dispatch(takeoff_planes.front());
            takeoff_planes.pop();
        }
    }
    bool push_emergency_plane(RunwayBase& rw, size_t& landing_cnt) {
        if (!emergency_planes.empty() && landing_cnt < 3) {  // emergency landing queue --> runways
            rw.dispatch(emergency_planes.top());
            emergency_planes.pop();
            landing_cnt++;
            return true;
        }
        return false;
    }

    array<unique_ptr<RunwayBase>, 3> arr;

    priority_queue<LandingPlane> emergency_planes;
    queue<TakeoffPlane>          takeoff_planes;

    ofstream queue_info;
    ofstream stuff_info;
};

int main() {
    ofstream fs("left_time.csv", ios::out | ios::trunc);
    fs << "left_time" << endl;
    // read file
    Airport airport;
    // add planes
    uniform_int_distribution<> u(2, 20);
    random_device              rd;
    for (auto i : views::iota(0, 25)) {
        airport.addPlane(TakeoffPlane{ 0 });
        size_t left_time = u(rd);
        fs << left_time << endl;
        airport.addPlane(LandingPlane(0, left_time));
    }

    while (!airport.empty()) {
        auto [rw1, rw2, em] = move(airport.run());
        // clang-format off
        cout << format("time {}:\nrunway1:{:8} runway2:{:8} emergency:{:8}\n",
            ++current_time,
            rw1 ? to_string(rw1->id) : "empty", 
            rw2 ? to_string(rw2->id) : "empty", 
            em ? to_string(em->id) : "empty");
        // clang-format on
    }
}