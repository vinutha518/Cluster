
#include "UDS_DTC.h"
#include "UDSS.h"
#include "UDSS_APP.h"

#define SET			1U
#define CLEAR		0U
extern DTC DTC_IVN_FEATURE_ERR_E805_Buff;

void Check_Valid_Feature_Code(void)
{
    if((Feature_code_arr[0] & 0x1) == 0x00)
    { 
        DTC_IVN_FEATURE_ERR_E805_Buff.BYTEs[3] = DTC_CNF_TEST_FAIL; /*DTC currently present*/
    }
    else
    {
		if(DTC_IVN_FEATURE_ERR_E805_Buff.BYTEs[3] == DTC_CNF_TEST_FAIL)
		{
			DTC_IVN_FEATURE_ERR_E805_Buff.BYTEs[3] = DTC_CNF; /*DTC previously present*/
		}
    }
}
