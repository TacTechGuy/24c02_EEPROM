#ifndef UNITY_CONFIG_H
#define UNITY_CONFIG_H

// Declare these as pure C functions so the Unity C compiler can read them safely
#ifdef __cplusplus
extern "C" {
#endif

void unityOutputStart(void);
void unityOutputChar(char c);
void unityOutputFlush(void);
void unityOutputComplete(void);

#ifdef __cplusplus
}
#endif

// Link Unity's internal engine macros to our functions
#define UNITY_OUTPUT_START()    unityOutputStart()
#define UNITY_OUTPUT_CHAR(c)    unityOutputChar(c)
#define UNITY_OUTPUT_FLUSH()    unityOutputFlush()
#define UNITY_OUTPUT_COMPLETE() unityOutputComplete()

#endif

