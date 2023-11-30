﻿const THREE = require('three');
const OBJLoader = require('three-obj-mtl-loader').OBJLoader;


const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
const renderer = new THREE.WebGLRenderer();

renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);

// Create an OBJLoader and load the model
const objLoader = new OBJLoader();
objLoader.load(
    'model/HORSE.obj',
    (object) => {
        scene.add(object);
        animate();
    },
    (xhr) => {
        console.log((xhr.loaded / xhr.total * 100) + '% loaded');
    },
    (error) => {
        console.log('An error happened');
    }
);

camera.position.z = 5;

function animate() {
    requestAnimationFrame(animate);
    renderer.render(scene, camera);
}
