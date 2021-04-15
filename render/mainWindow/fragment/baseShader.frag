#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;
layout(location = 0) out vec4 outColor;

void drawCircle(vec2 center, float radius, bool gradient) {
    float centerDistance = distance(gl_FragCoord.xy, center.xy);
    if (centerDistance <= radius) {
        float color = 1.0;
        if (gradient) {
            color = 1.-centerDistance/radius;
        } 
        outColor = vec4(color, color, color, 1.0);
    } else {
        outColor = vec4(fragColor, 1.0);
    }
}

void drawQuatrefoil(vec2 center, bool gradient) {
    float x = gl_FragCoord.x-800;
    float y = gl_FragCoord.y-800;
    float centerDistance = pow(pow(x,2)+pow(y,2),3);
    float radius = 3200000*pow(x*y,2);
    if (centerDistance <= radius) {
        float color = 1.0;
        if (gradient) {
            color = 1.-centerDistance/radius;
        } outColor = vec4(color, color, color, 1.0);
    } else {
        outColor = vec4(fragColor, 1.0);
    }
}

void drawCardioid(vec2 center, float a, bool gradient) {
    float x = gl_FragCoord.x-800;
    float y = gl_FragCoord.y-800;
    float centerDistance = pow(pow(x,2)+pow(y,2)-a*x,2);
    float radius = pow(a,2)*(pow(x,2)+pow(y,2));

    if (pow(pow(x,2)+pow(y,2)-a*x,2)<=pow(a,2)*(pow(x,2)+pow(y,2))) {
        float color = 1.0;
        if (gradient) {
            color = 1.-centerDistance/radius;
        } outColor = vec4(color, color, color, 1.0);
    } else {
        outColor = vec4(fragColor, 1.0);
    }
}

void drawLemniscate(vec2 center, float d, bool gradient) {
    float x = gl_FragCoord.x-800;
    float y = gl_FragCoord.y-800;
    float centerDistance = pow(pow(x,2)+pow(y,2),2);
    float radius = 2*pow(d,2)*(pow(x,2)-pow(y,2));

    if (pow(pow(x,2)+pow(y,2),2)<=2*pow(d,2)*(pow(x,2)-pow(y,2))) {
        float color = 1.0;
        if (gradient) {
            color = 1.-centerDistance/radius;
        } outColor = vec4(color, color, color, 1.0);
    } else {
        outColor = vec4(fragColor, 1.0);
    }
}

void main() {
    drawCircle(vec2(800,800),400,true);
    //drawCardioid(vec2(800,800),400,true);
    //drawLemniscate(vec2(800,800),400,true);
    //drawQuatrefoil(vec2(800,800),true);
}