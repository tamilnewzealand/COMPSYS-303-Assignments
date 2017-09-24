/*
 * sccharts.h
 *
 *  Created on: 9 செப்., 2017
 *      Author: சகாயன் சிற்சபேசன்
 */

#ifndef SCCHARTS_H_
#define SCCHARTS_H_

// Function Definitions
void reset();
void tick();

// Inputs
extern char ASense;
extern char VSense;

extern char AVI_start;
extern char PVARP_start;
extern char VRP_start;
extern char AEI_start;
extern char LRI_start;
extern char URI_start;

extern char AVI_stop;
extern char AEI_stop;
extern char LRI_stop;

// Outputs
extern char AVITO;
extern char PVARPTO;
extern char VRPTO;
extern char AEITO;
extern char LRITO;
extern char URITO;

extern char VPace;
extern char APace;

#endif /* SCCHARTS_H_ */
