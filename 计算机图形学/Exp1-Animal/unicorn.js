var THREE = require('three');
var OBJLoader = require('three-obj-mtl-loader').OBJLoader;
var scene = new THREE.Scene();
var camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
var renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);
// Create an OBJLoader and load the model
var objLoader = new OBJLoader();
objLoader.load('model/HORSE.obj', function (object) {
    scene.add(object);
    animate();
}, function (xhr) {
    console.log((xhr.loaded / xhr.total * 100) + '% loaded');
}, function (error) {
    console.log('An error happened');
});
camera.position.z = 5;
function animate() {
    requestAnimationFrame(animate);
    renderer.render(scene, camera);
}
