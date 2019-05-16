/*
*global.h
*/

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#define ID_RECOGNITION_NAME_MAX			128
#define	CHINESE_SYM_GEN_TASK_NAME		"tChineseSym"
#define	CHINESE_SYM_GEN_TASK_PRI		100
#define	CHINESE_SYM_GEN_TASK_ST			10*1024*1024
#define	CHINESE_SYM_GEN_TASK_TIMEOUT	1000


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
void GlobalInit();
void GlobalExit();
#ifdef __cplusplus
}
#endif // __cplusplus
#endif // !__GLOBAL_H__

