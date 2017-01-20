#ifndef FIXED_H_INCLUDED
#define FIXED_H_INCLUDED

typedef signed int fix32 ;
typedef signed short fix16;

#define MAX_30 (1073741824)

#define multfix32(a,b) ((fix32)(((( signed long long)(a))*(( signed long long)(b))) >> 30)) //multiply two fixed 2:30
#define divfix32(a,b) ((fix32)((((signed long long)(a)<<30)/(b))))

#define float2fix32(a) ((fix32)((a)*1073741824.0)) // 2^30
#define fix2float32(a) ((float)(a)/1073741824.0)

#define linterp(weight, start, end) ((multfix32(MAX_30 - weight, start) + multfix32(weight, end)))

#endif // FIXED_H_INCLUDED
