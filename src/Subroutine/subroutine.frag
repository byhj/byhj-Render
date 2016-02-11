#version 430 core

layout (location = 0) out vec4 g_FragColor;

subroutine vec4 mySub(vec4 param);

subroutine(mySub) vec4 myFunc1(vec4 param)
{
   return param * vec4(1.0, 0.25, 0.25, 1.0);
}

subroutine(mySub) vec4 myFunc2(vec4 param)
{
   return param * vec4(0.25, 0.25, 1.0, 1.0);
}

subroutine uniform mySub subUniform;

void main(void)
{
   g_FragColor =  subUniform(vec4(1.0f));
}
