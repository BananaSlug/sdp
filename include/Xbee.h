/*
 * File: Xbee.h
 *
 * State machine for Xbee module.
 *
 */
#ifndef Xbee_H
#define Xbee_H



/***********************************************************************
 * PUBLIC DEFINITIONS                                                  *
 ***********************************************************************/
#define GROUND_STATION_XBEE 74
#define BOAT_XBEE 33

/**********************************************************************
 * PUBLIC FUNCTIONS                                                   *
 **********************************************************************/


/**********************************************************************
 * Function: Xbee_init()
 * @param An options bitfield.
 * @return none
 * @remark Initializes the GPS module.
 **********************************************************************/
uint8_t Xbee_init();

/**********************************************************************
 * Function: Xbee_isInitialized()
 * @return Whether the GPS was initialized.
 * @remark none
 **********************************************************************/
uint8_t Xbee_isInitialized(void);

/**********************************************************************
 * Function: Xbee_programInit()
 * @param Which Xbee is being programed, ground station or boat
 * @remark Puts the Xbee into API mode.
 **********************************************************************/
uint8_t Xbee_programApi();

/**********************************************************************
 * Function: Xbee_isApi()
 * @return Whether the Xbee was succesfully put into API mode
 * @remark none
 **********************************************************************/
uint8_t Xbee_isApi(void);

/**********************************************************************
 * Function: Xbee_programInit()
 * @param Which Xbee is being programed, ground station or boat
 * @remark Initializes an array with data required for sending a
 *  message in API mode.
 **********************************************************************/
void Xbee_initSend();

/**********************************************************************
 * Function: Xbee_sendString()
 * @param data that needs to be transmitted, split up into proper bytes
 * @remark Adds data to the sendArray, than sends that data over the 
 *  UART.
 **********************************************************************/
void Xbee_sendString(uint8_t data);


/**********************************************************************
 * Function: Xbee_recieveData()
 * @remark Beings taking in data, once a packet has been fully read in,
 * we will analyze that packet for it's useful information, namely
 * the GPS cordinate we should be driving towards.
 **********************************************************************/
void Xbee_recieveData();

#endif