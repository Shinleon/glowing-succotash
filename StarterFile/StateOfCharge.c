#include <stdlib.h>
#include <stdbool.h>
#include "StateOfCharge.h"



float computeSoc(const float* volt, const float* current, const float* temp)
{
    float resistance = 0.5;
    float open_cVolt = volt + resistance * current; 
    float left_v = -1; //hold the index of the largest v less than volt
    float right_v = -1; //hold the index of the smallest v larger than volt
    float lower_temp = -1; //hold the index of the smallest t larger than temp
    float upper_temp = -1; //hold the index of the largest t smaller than temp

    for(int i = 0; i < volt_length; i++)
    {
        if(open_cVolt < voltages_soc[i])
        {
            if (i == 0)
            {
                return 0;
            }
            else if(right_v == -1)
            {
                right_v = i;
            }
        }
        if(open_cVolt > voltages_soc[i])
        {
            if (i == volt_length - 1)
            {
                return 100;
            }
            else if(left_v == -1)
            {
                left_v = i;
            }
        }
    }

    for(int i = 0; i < temp_length; i++)
    {
        if(temp < temperatures_soc[i])
        {
            if (i == 0)
            {
                return -1; //temp is below -10
            }
            else if(right_v == -1)
            {
                upper_temp = i;
            }
        }
        if(temp > temperatures_soc[i])
        {
            if (i == volt_length - 1)
            {
                return -1;   //temp is above 45
            }
            else if(left_v == -1)
            {
                lower_temp = i;
            }
        }
    }
    //interpolation at upper temp w respect to voltage
    float voltage_ratio = (open_cVolt - voltages_soc[left_v])/(voltages_soc[right_v]-voltages_soc[left_v]);
    float inter_upper = voltage_ratio *
                        (percent_soc[lower_temp][right_v] - percent_soc[lower_temp][left_v])
                         + percent_soc[lower_temp][left_v];
    //interpolation at lower temp w respect to voltage
    float inter_lower = voltage_ratio *
                        (percent_soc[upper_temp][right_v] - percent_soc[upper_temp][left_v]) 
                         + percent_soc[upper_temp][left_v];
    
    //interpolate with respect to temperature
    float temp_ratio = (temp - temperatur_soc[lower_temp])/(temperatures_soc[upper_temp] - temperatures_soc[lower_temp]);
    float soc = inter_upper - temp_ratio*(inter_upper - inter_lower);
    return soc;
}
/************************************************************************
  * Function name: stateOfChargeTask
  * Function inputs: generic pointer to the updated state of charge data
  * Function outputs: ~
  * Function description: update state of charge using bi-linear interpolation
  * Author(s): Leonard Shin, Leika Yamada
  **********************************************************************/
void stateOfChargeTask ( void* socData ) {
  
    stateOfChargeData* data = (stateOfChargeData*) socData;
    data->soc = computeSoc(data->vTerminal, data->iTerminal, data->temp);
  return;
}
