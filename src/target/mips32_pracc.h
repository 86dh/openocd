/* SPDX-License-Identifier: GPL-2.0-or-later */

/***************************************************************************
 *   Copyright (C) 2008 by Spencer Oliver                                  *
 *   spen@spen-soft.co.uk                                                  *
 *                                                                         *
 *   Copyright (C) 2008 by David T.L. Wong                                 *
 *                                                                         *
 *   Copyright (C) 2011 by Drasko DRASKOVIC                                *
 *   drasko.draskovic@gmail.com                                            *
 ***************************************************************************/

#ifndef OPENOCD_TARGET_MIPS32_PRACC_H
#define OPENOCD_TARGET_MIPS32_PRACC_H

#include <target/mips32.h>
#include <target/mips_ejtag.h>

#define MIPS32_PRACC_FASTDATA_AREA		0xFF200000
#define MIPS32_PRACC_FASTDATA_SIZE		16
#define MIPS32_PRACC_BASE_ADDR			0xFF200000
#define MIPS32_PRACC_TEXT			0xFF200200
#define MIPS32_PRACC_PARAM_OUT			0xFF202000

#define PRACC_UPPER_BASE_ADDR			(MIPS32_PRACC_BASE_ADDR >> 16)
#define PRACC_MAX_CODE				(MIPS32_PRACC_PARAM_OUT - MIPS32_PRACC_TEXT)
#define PRACC_MAX_INSTRUCTIONS			(PRACC_MAX_CODE / 4)
#define PRACC_OUT_OFFSET			(MIPS32_PRACC_PARAM_OUT - MIPS32_PRACC_BASE_ADDR)

#define MIPS32_FASTDATA_HANDLER_SIZE		0x80
#define UPPER16(addr)				((addr) >> 16)
#define LOWER16(addr)				((addr) & 0xFFFF)
#define NEG16(v)				(((~(v)) + 1) & 0xFFFF)
#define SWAP16(v)				((LOWER16(v) << 16) | (UPPER16(v)))
/*#define NEG18(v) (((~(v)) + 1) & 0x3FFFF)*/

#define PRACC_BLOCK	128	/* 1 Kbyte */

struct mips32_common;

struct pa_list {
	uint32_t instr;
	uint32_t addr;
};

struct pracc_queue_info {
	struct mips_ejtag *ejtag_info;
	unsigned int isa;
	int retval;
	int code_count;
	int store_count;
	int max_code;		/* max instructions with currently allocated memory */
	struct pa_list *pracc_list;	/* Code and store addresses at dmseg */
};

struct mips32_common;

void pracc_queue_init(struct pracc_queue_info *ctx);
void pracc_add(struct pracc_queue_info *ctx, uint32_t addr, uint32_t instr);
void pracc_queue_free(struct pracc_queue_info *ctx);
int mips32_pracc_queue_exec(struct mips_ejtag *ejtag_info,
			    struct pracc_queue_info *ctx, uint32_t *buf, bool check_last);

int mips32_pracc_read_mem(struct mips_ejtag *ejtag_info,
		uint32_t addr, int size, int count, void *buf);
int mips32_pracc_write_mem(struct mips_ejtag *ejtag_info,
		uint32_t addr, int size, int count, const void *buf);
int mips32_pracc_fastdata_xfer(struct mips_ejtag *ejtag_info, struct working_area *source,
		int write_t, uint32_t addr, int count, uint32_t *buf);

int mips32_pracc_read_regs(struct mips32_common *mips32);
int mips32_pracc_write_regs(struct mips32_common *mips32);

/**
 * \b mips32_cp0_read
 *
 * Simulates mfc0 ASM instruction (Move From C0),
 * i.e. implements copro C0 Register read.
 *
 * @param[in] ejtag_info
 * @param[in] val Storage to hold read value
 * @param[in] cp0_reg Number of copro C0 register we want to read
 * @param[in] cp0_sel Select for the given C0 register
 *
 * @return ERROR_OK on Success, ERROR_FAIL otherwise
 */
int mips32_cp0_read(struct mips_ejtag *ejtag_info,
		uint32_t *val, uint32_t cp0_reg, uint32_t cp0_sel);

/**
 * \b mips32_cp0_write
 *
 * Simulates mtc0 ASM instruction (Move To C0),
 * i.e. implements copro C0 Register read.
 *
 * @param[in] ejtag_info
 * @param[in] val Value to be written
 * @param[in] cp0_reg Number of copro C0 register we want to write to
 * @param[in] cp0_sel Select for the given C0 register
 *
 * @return ERROR_OK on Success, ERROR_FAIL otherwise
 */
int mips32_cp0_write(struct mips_ejtag *ejtag_info,
		uint32_t val, uint32_t cp0_reg, uint32_t cp0_sel);

/**
 * mips32_cp1_control_read
 *
 * @brief Simulates cfc1 ASM instruction (Move Control Word From Floating Point),
 * i.e. implements copro C1 Control Register read.
 *
 * @param[in] ejtag_info
 * @param[in] val Storage to hold read value
 * @param[in] cp1_c_reg Number of copro C1 control register we want to read
 *
 * @return ERROR_OK on Success, ERROR_FAIL otherwise
 */
int mips32_cp1_control_read(struct mips_ejtag *ejtag_info,
		uint32_t *val, uint32_t cp1_c_reg);

static inline void pracc_swap16_array(struct mips_ejtag *ejtag_info, uint32_t *buf, int count)
{
	if (ejtag_info->isa && ejtag_info->endianness)
		for (int i = 0; i != count; i++)
			buf[i] = SWAP16(buf[i]);
}

#endif /* OPENOCD_TARGET_MIPS32_PRACC_H */
