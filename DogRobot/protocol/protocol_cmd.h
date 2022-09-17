#ifndef __PROTOCOL_CMD_H
#define __PROTOCOL_CMD_H


//索引
#define LEN_INDEX_VAL        0x4u     // 包长索引值（4字节）
#define CMD_INDEX_VAL        0x8u     // 命令索引值（1字节）
#define PROT_FRAME_LEN_CHECKSUM    2// 校验位长度 为2个字节

#define FRAME_HEADER     0x59485A53    // 帧头

#define COMPOUND_32BIT(data)        (((*(data-0) << 24) & 0xFF000000) |\
                                     ((*(data-1) << 16) & 0x00FF0000) |\
                                     ((*(data-2) <<  8) & 0x0000FF00 ) |\
                                     ((*(data-3) <<  0) & 0x000000FF))      // 合成为一个字


/**************通讯指令*******************/

/* 空指令 */
#define CMD_NONE               0xFF     // 空指令
#define CMD_Exit_Align         0x00     //停止校准指令
#define CMD_Servor_align       0x01     //舵机校准指令
#define CMD_Dog_Servor_ctrl    0X02     //舵机设置指令
#define CMD_Dog_XY_ctrl        0x03     //单腿设置指令
#define CMD_Gait_ctrl          0x04     //进入步态指令
#define CMD_Gait_trot_set_ctrl 0x05   //trot步态参数设置
#define CMD_Gait_dir_set       0x06     //gait步态方向设置指令
#endif 

