// 画圆
// 半径r 面数m 度数c
function getCircleVertex(r, m, c) {
    var arr = [];
    var addAng = c / m;
    var angle = 0;
    for (var i = 0; i < m; i++) {
        arr.push(Math.sin(Math.PI / 180 * angle) * r, Math.cos(Math.PI / 180 * angle) * r, 0, 1.0);
        arr.push(0.0, 0.0, 0.0, 1.0);
        angle = angle + addAng;
        arr.push(Math.sin(Math.PI / 180 * angle) * r, Math.cos(Math.PI / 180 * angle) * r, 0, 1.0);
    }
    return arr;
}
window.onload = function init () {
    var canvas = document.getElementById( "gl-canvas" );

    var gl = WebGLUtils.setupWebGL( canvas );
    if ( !gl ) { alert( "WebGL isn't available" ); }

    // 设置窗口大小
    gl.viewport( 0, 0, canvas.width, canvas.height );
    gl.clearColor( 0.0, 0.0, 0.0, 1.0 );

    // 初始化着色器
    var program = initShaders( gl, "vertex-shader", "fragment-shader" );
    gl.useProgram( program );

    // 获取vPosition变量的存储位置
    var vPosition = gl.getAttribLocation(program, "vPosition");
    if (vPosition < 0) {
        console.log('Failed to get the storage location of vPosition');
        return;
    }

    // 获取u_transMat变量的存储位置
    var u_transMat = gl.getUniformLocation(program, "u_transMat");
    if (u_transMat < 0) {
        console.log('Failed to get the storage location of u_transMat');
        return;
    }

    // 获取u_FragColor变量的存储位置
    var u_FragColor = gl.getUniformLocation(program, 'u_FragColor');
    if (!u_FragColor) {
        console.log('Failed to get the storage location of u_FragColor');
        return;
    }

    var colors = [
        [1.0, 0.843, 0.0, 1.0], //金黄色
        [1.0, 0.647, 0.0, 1.0], //橙色
        [0.824, 0.412, 0.118, 1.0], //巧克力色
        [0.0, 0.0, 0.0, 1.0] //黑色
    ];
    // 画鸡嘴（三角形）
    vertices = [
        -0.392, -0.04, 0.0, 1.0,
        -0.42, -0.08, 0.0, 1.0,
        -0.36, -0.08, 0.0, 1.0
    ];
    mat4 = new Float32Array([
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    ]);
    // 创建缓存
    var buffer = gl.createBuffer(); // 为顶点创建的缓存
    gl.bindBuffer(gl.ARRAY_BUFFER, buffer); // 绑定缓冲区
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
    gl.vertexAttribPointer(vPosition, 4, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(vPosition);

    gl.uniform4f(u_FragColor, colors[1][0], colors[1][1], colors[1][2], colors[1][3]);
    gl.uniformMatrix4fv(u_transMat, false, mat4);

    gl.clear(gl.COLOR_BUFFER_BIT);
    gl.drawArrays(gl.TRIANGLES, 0, 3);
// 画鸡头（圆）
var ms = 180; // 组成圆的划分三角形个数
var vertices = getCircleVertex(0.1, ms, 360);

var Tx = -0.3;
var Ty = 0.0;
var Tz = 0.0;
var mat4 = new Float32Array([
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    Tx, Ty, Tz, 1.0
]);

gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW); // 向缓冲区写入顶点数据
gl.vertexAttribPointer(vPosition, 4, gl.FLOAT, false, 0, 0);
gl.uniform4f(u_FragColor, colors[0][0], colors[0][1], colors[0][2], colors[0][3]);
gl.uniformMatrix4fv(u_transMat, false, mat4);
gl.drawArrays(gl.TRIANGLES, 0, ms*3);
// 画鸡内侧翅膀（0.1半圆）
vertices = getCircleVertex(0.1, ms, 180);
mat4 = new Float32Array([
    0.866, -0.5, 0.0, 0.0,
    0.5, 0.866, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    -0.05, 0.02, 0.0, 1.0
]);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
gl.vertexAttribPointer(vPosition, 4, gl.FLOAT, false, 0, 0);
gl.uniform4f(u_FragColor, colors[1][0], colors[1][1], colors[1][2], colors[1][3]);
gl.uniformMatrix4fv(u_transMat, false, mat4);
gl.drawArrays(gl.TRIANGLES, 0, ms*3);

// 画鸡身体（半圆）
vertices = getCircleVertex(0.2, ms, 180);
mat4 = new Float32Array([
    0.0, -1.0, 0.0, 0.0,
    1.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
]);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
gl.vertexAttribPointer(vPosition, 4, gl.FLOAT, false, 0, 0);
gl.uniform4f(u_FragColor, colors[0][0], colors[0][1], colors[0][2], colors[0][3]);
gl.uniformMatrix4fv(u_transMat, false, mat4);
gl.drawArrays(gl.TRIANGLES, 0, ms*3);

// 画鸡尾巴（60°扇形）
vertices = getCircleVertex(0.08, ms, 60);
mat4 = new Float32Array([
    0.5, -0.866, 0.0, 0.0,
    0.866, 0.5, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.2, 0.0, 0.0, 1.0
]);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
gl.vertexAttribPointer(vPosition, 4, gl.FLOAT, false, 0, 0);
gl.uniformMatrix4fv(u_transMat, false, mat4);
gl.drawArrays(gl.TRIANGLES, 0, ms*3);

// 画鸡外侧翅膀（0.15半圆）
vertices = getCircleVertex(0.15, ms, 180);
mat4 = new Float32Array([
    0.707, -0.707, 0.0, 0.0,
    0.707, 0.707, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
]);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
gl.vertexAttribPointer(vPosition, 4, gl.FLOAT, false, 0, 0);
gl.uniform4f(u_FragColor, colors[1][0], colors[1][1], colors[1][2], colors[1][3]);
gl.uniformMatrix4fv(u_transMat, false, mat4);
gl.drawArrays(gl.TRIANGLES, 0, ms*3);

// 画鸡左腿（直线）
vertices = [
    -0.05, -0.19, 0.0, 1.0,
    -0.05, -0.25, 0.0, 1.0,
    -0.045, -0.195, 0.0, 1.0,
    -0.045, -0.25, 0.0, 1.0
];
mat4 = new Float32Array([
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
]);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
gl.vertexAttribPointer(vPosition, 4, gl.FLOAT, false, 0, 0);
gl.uniform4f(u_FragColor, colors[2][0], colors[2][1], colors[2][2], colors[2][3]);
gl.uniformMatrix4fv(u_transMat, false, mat4);
gl.drawArrays(gl.LINE_LOOP, 0, 4);

// 画鸡右腿（直线）
vertices = [
    0.05, -0.19, 0.0, 1.0,
    0.05, -0.25, 0.0, 1.0,
    0.045, -0.195, 0.0, 1.0,
    0.045, -0.25, 0.0, 1.0
];
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
gl.vertexAttribPointer(vPosition, 4, gl.FLOAT, false, 0, 0);
gl.uniformMatrix4fv(u_transMat, false, mat4);
gl.drawArrays(gl.LINE_LOOP, 0, 4);

// 画鸡左脚（三角形）
vertices = [
    -0.045, -0.25, 0.0, 1.0,
    -0.04, -0.28, 0.0, 1.0,
    -0.09, -0.28, 0.0, 1.0
];
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
gl.vertexAttribPointer(vPosition, 4, gl.FLOAT, false, 0, 0);
gl.uniformMatrix4fv(u_transMat, false, mat4);
gl.drawArrays(gl.TRIANGLES, 0, 3);

// 画鸡右脚（三角形）
vertices = [
    0.05, -0.25, 0.0, 1.0,
    0.0, -0.28, 0.0, 1.0,
    0.055, -0.28, 0.0, 1.0
];
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
gl.vertexAttribPointer(vPosition, 4, gl.FLOAT, false, 0, 0);
gl.uniformMatrix4fv(u_transMat, false, mat4);
gl.drawArrays(gl.TRIANGLES, 0, 3);

// 画鸡眼睛（圆）
var vertices = getCircleVertex(0.01, ms, 360);

var Tx = -0.35;
var Ty = -0.015;
var Tz = 0.0;
var mat4 = new Float32Array([
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    Tx, Ty, Tz, 1.0
]);

gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW); // 向缓冲区写入顶点数据
gl.vertexAttribPointer(vPosition, 4, gl.FLOAT, false, 0, 0);
gl.uniform4f(u_FragColor, colors[3][0], colors[3][1], colors[3][2], colors[3][3]);
gl.uniformMatrix4fv(u_transMat, false, mat4);
gl.drawArrays(gl.TRIANGLES, 0, ms*3);

// 菜单
    var mymenu = document.getElementById("mymenu");
    mymenu.addEventListener("click",function(){
        if (selectedOperation === "0") {
            // "Go forward"
        } else if (selectedOperation === "1") {
            // "Go backward"
        } else if (selectedOperation === "2") {
            // "Jump"
        }
    });
};


