/* SPDX-License-Identifier: Apache-2.0 */
#ifndef STAGE9_REGISTER_ACCESS_H
#define STAGE9_REGISTER_ACCESS_H

#include <stdint.h>
#include <stddef.h>

#define REG_PDOR 0x40U
#define REG_PSOR 0x44U
#define REG_PCOR 0x48U
#define REG_PTOR 0x4cU
#define REG_PDIR 0x50U
#define REG_PDDR 0x54U
#define REG_LED_MASK UINT32_C(0x400)

int register_access_init(void);
uintptr_t register_access_base(void);
/* 呼出し元が許可表・幅・値を検査してから、固定のoffsetだけを渡す。 */
uint32_t register_access_read(uint32_t offset);
void register_access_write(uint32_t offset, uint32_t value);

#endif
