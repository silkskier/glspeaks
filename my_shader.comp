#version 460

// The execution structure
layout (local_size_x = 1024) in;

// The buffers are provided via the tensors
layout(binding = 0) buffer bufT { float t[]; };
layout(binding = 1) buffer bufY { float y[]; };
layout(binding = 2) buffer bufDy { float dy[]; };
layout(binding = 3) buffer bufF { float f[]; };

layout(binding = 4) buffer bufShort { float short_buffer[]; }; //short temporary buffer

layout(binding = 5) buffer bufW          { float w[]; };
layout(binding = 6) buffer bufWy        { float wy[]; };
layout(binding = 7) buffer bufPusty    { float abcd[]; };
layout(binding = 8) buffer bufOutput    { float output_data[]; };
layout(binding = 9) buffer bufCosDx  { float cosdx[]; };
layout(binding = 10) buffer bufSinDx { float sindx[]; };

layout(binding = 11) buffer bufLong { float long_buffer[]; }; //long temporary buffer

layout(binding = 12) buffer bufBreakpoints { uint breakpoints[]; };

layout(binding = 13) buffer bufWsum { float wsum[]; };

layout(binding = 14) buffer bufWnorm { float wnorm[]; };

layout(binding = 15) buffer bufY_ { float Y_[]; };



float w_normalisation (uint index){
float wsum_temp = 0;
for (uint i = breakpoints[index]; i < breakpoints[index+1]; i++){wsum_temp += w[i];}
for (uint i = breakpoints[index]; i < breakpoints[index+1]; i++){wnorm[i] = w[i]/wsum_temp;}
return wsum_temp;}


float calculate_Y (uint index){
float y_temp = 0;
for (uint i = breakpoints[index]; i < breakpoints[index+1]; i++){y_temp += wnorm[i] * y[i];}
return y_temp;}


float calculate_wy (uint index){						
float yy_temp = 0;

for (uint i = breakpoints[index]; i < breakpoints[index+1]; i++){
if (Y_[index] != 0){ long_buffer[i] = y[i] - Y_[index];}
else{long_buffer[i] = 0;}} //Subtract weighted mean

for (uint i = breakpoints[index]; i < breakpoints[index+1]; i++){
if (Y_[index] != 0){long_buffer[i] = yy_temp += wnorm[i] * long_buffer[i] * long_buffer[i];} 
else{yy_temp = 0;}} //Eq. (10)

for (uint i = breakpoints[index]; i < breakpoints[index+1]; i++){wy[i] = wnorm[i] * (y[i] - yy_temp);} //attach weights
return yy_temp;}








void main() {


    uint index_x = gl_GlobalInvocationID.x;

float YY[8192]; //declares YY array



float step_size = f[1] - f[0];





    w[index_x] = 1 / (dy[index_x] * dy[index_x]); //calculates "w" array


wsum[index_x] = w_normalisation(index_x); //calculates wsum and normalised "w" array

Y_[index_x] = calculate_Y(index_x);  //Eq. (7) (line 46)

short_buffer[index_x] = calculate_wy(index_x); //Subtracts weighted mean

YY[index_x] = short_buffer[index_x]; //Eq. (10) (line 52)

cosdx[index_x] = cos(t[index_x] * step_size * 6.2831853);
sindx[index_x] = sin(t[index_x] * step_size * 6.2831853); 

float     C, S, YC, YS, CC, SS, CS, D, tmp; //declares variables



long_buffer[index_x] = 0; //nie działa


}
