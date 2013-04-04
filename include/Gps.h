/**
 * @file    Gps.h
 * @author  David Goodman
 *
 * @brief
 * Sensor interface for the GPS.
 *
 * @details 
 * Module that wraps the GPS in a state machine, which ocassionally
 * pulls location and velocity data from the GPS over UART. Also, this
 * module can be configured to communicate with a command station to
 * reduce error and improve accuracy.
 *
 * @note
 *  The longitude and latitude are both in degrees scaled by 1e-7.
 *  The heading is in degrees scaled by 1e-5.
 *  The velocity is in cm/s.
 * 
 * @date January 1, 2013, 1:25 AM   -- Created.
 */
#ifndef Gps_H
#define Gps_H


/***********************************************************************
 * PUBLIC DEFINITIONS                                                  *
 ***********************************************************************/
#define USE_GEOCENTRIC_COORDINATES  // uses GEODETIC if not defined


#define PI                      3.14159265359f
#define DEGREE_TO_RADIAN        ((float)PI/180.0)
#define RADIAN_TO_DEGREE        ((float)180.0/PI)

#define DEGREE_TO_NEDFRAME(deg) (-deg + 90.0)

/***********************************************************************
 * PUBLIC TYPEDEFS
 ***********************************************************************/

// Geodetic (lat, lon, alt) coordinate
typedef struct oGeodeticCoord {
    float lat, lon, alt;
} GeodeticCoordinate;

// Geocentric (ECEF) coordinate
typedef struct oGeocentricCoord {
    float x, y, z;
} GeocentricCoordinate;

// Local (NED) coordinate
typedef struct oLocalCoord {
    float n, e, d;
} LocalCoordinate;

// Course vector, where d is distance and yaw is degrees from North
typedef struct oCourseVector {
    float d, yaw;
} CourseVector;


/***********************************************************************
 * PUBLIC FUNCTIONS
 ***********************************************************************/

/**********************************************************************
 * Function: GPS_init
 * @return none
 * @remark Initializes the GPS.
 **********************************************************************/
BOOL GPS_init();

/**********************************************************************
 * Function: GPS_isInitialized
 * @return Whether the GPS was initialized.
 * @remark none
 **********************************************************************/
BOOL GPS_isInitialized();

/**********************************************************************
 * Function: GPS_runSM
 * @return None
 * @remark Executes the GPS's currently running state.
 **********************************************************************/
void GPS_runSM();

/**********************************************************************
 * Function: GPS_hasFix
 * @return TRUE if a lock has been obtained.
 * @remark
 **********************************************************************/
BOOL GPS_hasFix();

/**********************************************************************
 * Function: GPS_getPosition
 * @return The GPS's current position, in either geodetic coordinates
 *  (latitude, longitude, altitude in degrees), or geocentric coordinates
 *  (ECEF: x, y, z in meters from the earth's center).
 * @remark
 **********************************************************************/
#ifdef USE_GEOCENTRIC_COORDINATES
GeocentricCoordinate GPS_getPosition();
#else
GeodeticCoordinate GPS_getPosition();
#endif

/**********************************************************************
 * Function: GPS_getNorthVelocity
 * @return Returns the current velocity in the north direction.
 * @remark Centimeters per second in the north direction.
 **********************************************************************/
int32_t GPS_getNorthVelocity();

/**********************************************************************
 * Function: GPS_getEastVelocity
 * @return Returns the current velocity in the east direction.
 * @remark Centimeters per second in the east direction.
 **********************************************************************/
int32_t GPS_getEastVelocity();


/**********************************************************************
 * Function: GPS_getHeading
 * @return Returns the current heading in degrees scaled 1e-5.
 * @remark In degrees scaled by 1e-5.
 **********************************************************************/
int32_t GPS_getHeading();


/**********************************************************************
 * Function: GPS_isConnected
 * @return Returns true if GPS data seen in last 5 seconds.
 * @remark
 **********************************************************************/
int32_t GPS_isConnected();



/***********************************************************************
 * Library FUNCTIONS
 ***********************************************************************/

/**
 * Function: convertENU2ECEF
 * @param A pointer to a new ECEF coordinate variable to save result into.
 * @param East component in meters.
 * @param North component in meters.
 * @param Up component in meters.
 * @return None.
 * @remark Converts the given ENU vector into a ECEF coordinate.
 * @author David Goodman
 * @author MATLAB
 * @date 2013.03.10  
void convertENU2ECEF(GeocentricCoordinate *ecef, float east, float north, float up, float lat_ref,
    float lon_ref, float alt_ref);
*/

/**
 * Function: convertGeodetic2ECEF
 * @param A pointer to a new ECEF coordinate variable to save result into.
 * @param A pointer to a geodetic position.
 * @return None.
 * @remark Converts the given geodetic (LLA) coordinate into a geocentric (ECEF)
 *  coordinate in degrees.
 * @author David Goodman
 * @author MATLAB
 * @date 2013.03.10  */
void convertGeodetic2ECEF(GeocentricCoordinate *ecef, GeodeticCoordinate *lla);

/**
 * Function: convertECEF2Geodetic
 * @param A pointer to a new geodetic (LLA) coordinate variable to save result into.
 * @param ECEF X position.
 * @param ECEF Y position.
 * @param ECEF Z position.
 * @return None.
 * @remark Converts the given ECEF coordinates into a geodetic coordinate in degrees.
 *  Note that x=lat, y=lon, z=alt.
 * @author David Goodman
 * @author MATLAB
 * @date 2013.03.10 
void convertECEF2Geodetic(Coordinate *var, float ecef_x, float ecef_y, float ecef_z);
 */

/**
 * Function: projectEulerToNED
 * @param A pointer to a new NED coordinate variable to save result into.
 * @param Yaw in degrees from north.
 * @param Pitch in degrees from level.
 * @param Height in meters from target.
 * @return None.
 * @remark Projects a ray with the given height from the given yaw and
 *  pitch, and returns a NED for the intersection location.
 * @author David Goodman
 * @date 2013.03.10  */
void projectEulerToNED(LocalCoordinate *ned, float yaw, float pitch, float height);


/**
 * Function: getCourseVector
 * @param A pointer to a new NED vector variable to save result into.
 * @param A pointer to a NED reference position (current position).
 * @param A poimter to a NED position (desired position).
 * @return None.
 * @remark Calculates a vector (magnitude and angle) pointing from
 *  the current NED position to the desired NED position. Note that
 *  the down component (z) is not used.
 * @author David Goodman
 * @date 2013.04.02  */
void getCourseVector(CourseVector *course, LocalCoordinate *ned_cur,
        LocalCoordinate *ned_des);


/**
 * Function: convertECEF2NED
 * @param A pointer to a new NED vector variable to save result into.
 * @param A pointer to an ECEF position (current position).
 * @param A pointer to an ECEF reference position.
 * @param A pointer to the same referemce position, but in geodetic coords.
 * @return None.
 * @remark Converts the ECEF position into a NED vector starting from the
 *  geodetic reference point. Note that this function does not use the down
 *  component (z), though it will be used to calculate the north component.
 * @author David Goodman
 * @date 2013.04.02  */
void convertECEF2NED(LocalCoordinate *ned, GeocentricCoordinate *ecef_cur,
    GeocentricCoordinate *ecef_ref, GeodeticCoordinate *geo_ref);

#endif
