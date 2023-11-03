import * as THREE from 'three';

var scene, camera, renderer;
var horseModel, wingTexture, horseTexture;

function init() {
    scene = new THREE.Scene();
    camera = new THREE.PerspectiveCamera(75, 800 / 600, 0.1, 1000);
    renderer = new THREE.WebGLRenderer({ canvas: document.getElementById('webglCanvas') });
    renderer.setSize(800, 600);


    camera.position.z = 5;

    var loader = new THREE.OBJLoader();
    var wingTextureLoader = new THREE.TextureLoader();
    var horseTextureLoader = new THREE.TextureLoader();

    var loadModel = new Promise(function (resolve) {
        loader.load('./model/HORSE.obj', function (obj) {
            horseModel = obj;
            scene.add(horseModel);

            horseModel.position.set(0, 0, 0);
            horseModel.scale.set(0.1, 0.1, 0.1);

            resolve();
        });
    });

    var loadWingTexture = new Promise(function (resolve) {
        wingTextureLoader.load('./model/翅膀.tga', function (texture) {
            wingTexture = texture;
            resolve();
        });
    });

    var loadHorseTexture = new Promise(function (resolve) {
        horseTextureLoader.load('./model/火马.tga', function (texture) {
            horseTexture = texture;
            resolve();
        });
    });

    Promise.all([loadModel, loadWingTexture, loadHorseTexture]).then(function () {
        render();
    });
}

function render() {
    requestAnimationFrame(render);

    if (horseModel && wingTexture && horseTexture) {
        horseModel.rotation.y += 0.01;
        renderer.render(scene, camera);
    }
}

init();
