// 画圆
// 半径r 面数m 度数c
function getCircleVertex(r, m, c) {
    var arr = [];
    var addAng = c / m;
    var angle = 0;
    for (var i = 0; i < m; i++) {
        arr.push(
            vec2(Math.sin(Math.PI / 180 * angle) * r,
                Math.cos(Math.PI / 180 * angle) * r));
        arr.push(vec2(0.0, 0.0));
        angle = angle + addAng;
        arr.push(
            vec2(Math.sin(Math.PI / 180 * angle) * r,
                Math.cos(Math.PI / 180 * angle) * r));
    }
    return arr;
}
var vertices;
var colors;
var transMat = mat4(1);
var u_initTransMat;
var u_FragColor;
var u_transMat;
var gl;

window.onload = () => {
    var canvas = document.getElementById('gl-canvas');

    gl = WebGLUtils.setupWebGL(canvas);
    if (!gl) {
        alert('WebGL isn\'t available');
    }

    // 设置窗口大小
    gl.viewport(0, 0, canvas.width, canvas.height);
    gl.clearColor(1.0, 1.0, 1.0, 1.0);

    // 初始化着色器
    var program = initShaders(gl, 'vertex-shader', 'fragment-shader');
    gl.useProgram(program);

    u_transMat = gl.getUniformLocation(program, 'u_transMat');
    if (u_transMat < 0) {
        console.log('Failed to get the storage location of u_transMat');
        return;
    }
    u_initTransMat = gl.getUniformLocation(program, 'u_initTransMat');
    if (u_initTransMat < 0) {
        console.log('Failed to get the storage location of u_initTransMat');
        return;
    }

    u_FragColor = gl.getUniformLocation(program, 'u_FragColor');
    if (!u_FragColor) {
        console.log('Failed to get the storage location of u_FragColor');
        return;
    }

    colors = [
        vec4(1.0, 0.843, 0.0, 1.0),      // 金黄色
        vec4(1.0, 0.647, 0.0, 1.0),      // 橙色
        vec4(0.824, 0.412, 0.118, 1.0),  // 巧克力色
        vec4(0.0, 0.0, 0.0, 1.0)         // 黑色
    ];
    vertices = [
        // 鸡嘴
        vec2(-0.392, -0.04),
        vec2(-0.42, -0.08),
        vec2(-0.36, -0.08),
        // 鸡头
        ...getCircleVertex(0.1, 180, 360),
        // 内侧翅膀 
        ...getCircleVertex(0.1, 180, 180),
        // 身体 
        ...getCircleVertex(0.2, 180, 180),
        // 尾巴 
        ...getCircleVertex(0.08, 180, 60),
        // 外侧翅膀 
        ...getCircleVertex(0.15, 180, 180),
        // 左腿 
        vec2(-0.05, -0.19),
        vec2(-0.05, -0.25),
        vec2(-0.045, -0.195),
        vec2(-0.045, -0.25),
        // 右腿 
        vec2(0.05, -0.19),
        vec2(0.05, -0.25),
        vec2(0.045, -0.195),
        vec2(0.045, -0.25),
        // 左脚
        vec2(-0.045, -0.25),
        vec2(-0.04, -0.28),
        vec2(-0.09, -0.28),
        // 右脚 start index 4822 + 3 * 2 = 4828
        vec2(0.05, -0.25),
        vec2(0.0, -0.28),
        vec2(0.055, -0.28),
        // 眼睛 start index 4828 + 3 * 2 = 4834
        ...getCircleVertex(0.01, 180, 360)
    ]

    let isDragging = false;
    let dragStartX, dragStartY;
    const canvasRect = canvas.getBoundingClientRect();

    canvas.addEventListener('mousedown', (event) => {
        isDragging = true;
        dragStartX = (event.clientX - canvasRect.left) / canvas.width * 2 - 1;
        dragStartY = 1 - (event.clientY - canvasRect.top) / canvas.height * 2;
    });
    canvas.addEventListener('mousemove', (event) => {
        if (isDragging) {
            const mouseX = (event.clientX - canvasRect.left) / canvas.width * 2 - 1;
            const mouseY = 1 - (event.clientY - canvasRect.top) / canvas.height * 2;
            const deltaX = mouseX - dragStartX;
            const deltaY = mouseY - dragStartY;
            transMat[0][3] += deltaX;
            transMat[1][3] += deltaY;
            dragStartX = mouseX;
            dragStartY = mouseY;
        }
    });

    canvas.addEventListener('mouseup', () => {
        isDragging = false;
    });

    canvas.addEventListener('dblclick', () => {
        transMat[0][0] *= -1
        
    });

    // 创建缓存

    var buffer = gl.createBuffer();          // 为顶点创建的缓存
    gl.bindBuffer(gl.ARRAY_BUFFER, buffer);  // 绑定缓冲区
    gl.bufferData(gl.ARRAY_BUFFER, flatten(vertices), gl.STATIC_DRAW);
    gl.vertexAttribPointer(0, 2, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(0);

    var mymenu = document.getElementById("mymenu");
    let intervalId = null
    mymenu.addEventListener("click", () => {
        clearInterval(intervalId)
        switch (mymenu.options[mymenu.selectedIndex].value) {
            case "1":
                intervalId = setInterval(leftMove, 50)
                break
            case "2":
                intervalId = setInterval(rightMove, 50)
                break
            case "3":
                intervalId = setInterval(jump, 1000)
        }
    });

    render()
};

window.onkeydown = (event) => {
    switch (event.key) {
        case "ArrowLeft":
                leftMove()
            break
        case "ArrowRight":
                rightMove()
            break
        case "ArrowUp":
            jump()
    }

}

function leftMove() {
    transMat[0][3] -= transMat[0][0] * 0.01
}

function rightMove() {
    transMat[0][3] += transMat[0][0] * 0.01
}

function jump() {
    let t = 0
    let oldY = transMat[1][3]
    const f = (v) => -Math.pow(v - 5, 2) + 25
    let intervalId = setInterval(() => {
        transMat[1][3] = oldY + f(t) * 0.01
        if (++t == 10) {
            clearInterval(intervalId);
            t = 0
            transMat[1][3] = oldY
        }

    }, 50);
}

function render() {
    gl.clear(gl.COLOR_BUFFER_BIT);
    var index = 0;
    gl.uniformMatrix4fv(u_transMat, false, flatten(transMat));
    // mouth

    gl.uniform4f(
        u_FragColor, colors[1][0], colors[1][1], colors[1][2], colors[1][3]);
    gl.uniformMatrix4fv(u_initTransMat, false, flatten(mat4(1)));
    gl.drawArrays(gl.TRIANGLES, index, 3);
    index += 3

    // head
    var Tx = -0.3;
    var Ty = 0.0;
    var Tz = 0.0;
    var _mat4 = mat4(
        1.0, 0.0, 0.0, Tx,
        0.0, 1.0, 0.0, Ty,
        0.0, 0.0, 1.0, Tz,
        0.0, 0.0, 0.0, 1.0
    );

    gl.uniform4f(
        u_FragColor, colors[0][0], colors[0][1], colors[0][2], colors[0][3]);
    gl.uniformMatrix4fv(u_initTransMat, false, flatten(_mat4));
    gl.drawArrays(gl.TRIANGLES, index, 180 * 3);
    index += 180 * 3

    // inner wings
    _mat4 = mat4(
        0.866, 0.5, 0.0, -0.05,
        -0.5, 0.866, 0.0, 0.02,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
    gl.uniform4f(
        u_FragColor, colors[1][0], colors[1][1], colors[1][2], colors[1][3]);
    gl.uniformMatrix4fv(u_initTransMat, false, flatten(_mat4));
    gl.drawArrays(gl.TRIANGLES, index, 180 * 3);
    index += 180 * 3

    // body
    _mat4 = mat4(
        0.0, 1.0, 0.0, 0.0,
        -1.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
    gl.uniform4f(
        u_FragColor, colors[0][0], colors[0][1], colors[0][2], colors[0][3]);
    gl.uniformMatrix4fv(u_initTransMat, false, flatten(_mat4));
    gl.drawArrays(gl.TRIANGLES, index, 180 * 3);
    index += 180 * 3

    // tail
    _mat4 = mat4(
        0.5, 0.866, 0.0, 0.2,
        -0.866, 0.5, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    )
    gl.uniformMatrix4fv(u_initTransMat, false, flatten(_mat4));
    gl.drawArrays(gl.TRIANGLES, index, 180 * 3);
    index += 180 * 3

    // outter wings
    _mat4 = mat4(
        0.707, 0.707, 0.0, 0.0,
        -0.707, 0.707, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    )
    gl.uniform4f(
        u_FragColor, colors[1][0], colors[1][1], colors[1][2], colors[1][3]);
    gl.uniformMatrix4fv(u_initTransMat, false, flatten(_mat4));
    gl.drawArrays(gl.TRIANGLES, index, 180 * 3);
    index += 180 * 3

    // left leg & right leg
    gl.uniformMatrix4fv(u_initTransMat, false, flatten(mat4(1)));
    gl.uniform4f(
        u_FragColor, colors[2][0], colors[2][1], colors[2][2], colors[2][3]);
    gl.drawArrays(gl.LINE_LOOP, index, 4);
    index += 4
    gl.drawArrays(gl.LINE_LOOP, index, 4);
    index += 4

    // let feet & right & feet
    gl.drawArrays(gl.TRIANGLES, index, 3 * 2);
    index += 3 * 2

    // eyes
    Tx = -0.35;
    Ty = -0.015;
    Tz = 0.0;
    _mat4 = mat4(
        1.0, 0.0, 0.0, Tx,
        0.0, 1.0, 0.0, Ty,
        0.0, 0.0, 1.0, Tz,
        0.0, 0.0, 0.0, 1.0
    );
    gl.uniform4f(
        u_FragColor, colors[3][0], colors[3][1], colors[3][2], colors[3][3]);
    gl.uniformMatrix4fv(u_initTransMat, false, flatten(_mat4));
    gl.drawArrays(gl.TRIANGLES, index, 180 * 3);

    window.requestAnimationFrame(render)
}