#ifndef _TASK_CMD_LINE_COMMAND_H_
#define _TASK_CMD_LINE_COMMAND_H_

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Include ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
enum command_error_code
{
	CMDLINE_CLEAR_SCREEN = 9
};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* :::::::::: Power Control Command :::::::: */
int CMD_PWR_SOL_EN(int argc, char *argv[]);
int CMD_PWR_HTR_EN(int argc, char *argv[]);
int CMD_PWR_LAS_EN(int argc, char *argv[]);
int CMD_PWR_PHOT_EN(int argc, char *argv[]);

/* :::::::::: Heater Command :::::::: */
int CMD_HTR_SET(int argc, char *argv[]);

/* :::::::::: Solenoid Command :::::::: */
int CMD_SOL_CTL(int argc, char *argv[]);

/* :::::::::: NTC Command :::::::: */
int CMD_GET_TEMP_NTC(int argc, char *argv[]);
int CMD_EXP_START(int argc, char *argv[]);

/* :::::::::: Test Laser Command :::::::: */
int CMD_LASER_SET(int argc, char *argv[]);
int CMD_LASER_DAC(int argc, char *argv[]);

/* :::::::::: Test Photo Command :::::::: */
int CMD_PHOTO_SET(int argc, char *argv[]);

/* :::::::::: Ultility Command :::::::: */
int CMD_HELP(int argc, char *argv[]);
int CMD_CLEAR_SCREEN(int argc, char *argv[]);

int CMD_IO_CTRL(int argc, char *argv[]);
int CMD_HEATER_CTRL(int argc, char *argv[]);
int CMD_READ_TEMP(int argc, char *argv[]);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#endif /* _TASK_CMD_LINE_COMMAND_H_ */
