#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // Corrected header for string operations

#include "jsmn.h"

/********************************************************************************************************
*   EXAMPLE SETTINGS
********************************************************************************************************/

/// This number should be enough to parse all tokens in the desired json input. Read more about it at 
/// jsmn documentation.
#define dMAX_EXPECTED_JSON_TOKENS       64     
    
/// Enables or disables dynamic alloc
#define dEXAMPLE_WITH_DYNAMIC_ALLOC     true

/// Select the json input. Right now, we have 3 different samples:
/// 1 - Json with all data in the expected order
/// 2 - Json with all tada, but not in the expected order (wich works fine too)
/// 3 - Json with incomplete data
// #define dJSON_INPUT                     2     

// Example JSON data
const char jsonString[] = 
{
    "{\
        \"id\":\"2aa0a3bf-f03d-4973-b37a-2709d8f01994\",\
        \"equipmentSerialNumber\":\"96325\",\
        \"identificationHash\":null,\
        \"startSettingsDate\":\"2023-12-12T12:37:36.000Z\",\
        \"endSettingsDate\":null,\
        \"configuredValue\":\"18\",\
        \"isActiveParameter\":true,\
        \"startByAccountUserId\":\"19d9e3fd-6cf0-41c8-b356-51f7211e85e7\",\
        \"endByAccountUserId\":null,\
        \"parameterLanguage\":{\
            \"defaultLanguage\":\"BR\",\
            \"translations\":[\
                {\
                    \"locale\":\"BR\",\
                    \"dictionary\":[\
                        {\
                            \"key\":\"parameterTitle\",\
                            \"value\":\"Acesso\"\
                        }\
                    ]\
                }\
            ]\
        },\
        \"isActive\":true,\
        \"equipmentId\":\"f617795a-6539-4b1c-a6db-29e9d4514d95\",\
        \"productParameterSettingsId\":\"906c4fe4-ea45-44f8-a362-9e84ffec06c0\"\
    }"
};


int main()
{

    // Print the selected json file
    printf("Json file:\r\n\r\n");
    printf(jsonString);
    printf("\r\n");
    // Info that will be extracted from json
    char parameterTitle[64];  // Assuming a maximum length of 64 characters, adjust as needed
    int configuredValue = 0;

  

#if dEXAMPLE_WITH_DYNAMIC_ALLOC == true
    // Allocate the memory area for jsmn to run
    // OBS: if you do this plenty of times in your code, you can use dynamic allocation or
    // a fixed area. If you go the fixed area approach, just remember to memset(0) the working
    // buffer before use it
    jsmntok_t* jsonTokens = malloc(sizeof(jsmntok_t) * dMAX_EXPECTED_JSON_TOKENS);
#endif

    // The interpretation of json string by jsmn originally uses a go-to approach. To avoid it, we
    // will use a do-while(false) architecture just to be able to "break" anywere.
    do
    {
#if dEXAMPLE_WITH_DYNAMIC_ALLOC == true
        // Check if the malloc has done correctly
        if (jsonTokens == NULL)
        {
            printf("\r\nMalloc error of json parser");
            break;
        }
#endif

        // Initialization of the library
        jsmn_parser jsonParser_all;
        jsmn_init(&jsonParser_all);

        // Verifies how much json tokens are in the jsonString
        int tokens = jsmn_parse(&jsonParser_all, jsonString, strlen(jsonString), jsonTokens, dMAX_EXPECTED_JSON_TOKENS);

        // Handle error case
        if (tokens < 0)
        {
            printf("\r\nThis is not a json file or it is corrupted.");
            break;
        } // And also the empty case
        else if (tokens == 1 || jsonTokens[0].type != JSMN_OBJECT)
        {
            printf("\r\nThis json file is empty.");
            break;
        }
        else // Continue if is a valid json
        {
            printf("\r\nThis is a valid json file!");
        }

        // Here, the hunting for known tokens begins!
        // The start index is 1 because the token 0 is the json object itself
        for (int i = 1; i < tokens; i++)
        {
            // Inside the loop where you iterate through tokens
            if (json_equals(jsonString, &jsonTokens[i], "configuredValue"))
            {
                // Extract the configured value (assuming it's a primitive value)
                printf("\nConfigured Value token is here");
                int length = jsonTokens[i + 1].end - jsonTokens[i + 1].start;
                char value[length + 1];
                strncpy(value, jsonString + jsonTokens[i + 1].start, length);
                value[length] = '\0'; // Null-terminate the string
                configuredValue = atoi(value);
            }

            if (json_equals(jsonString, &jsonTokens[i], "value")) 
            {
                printf("\nParameter title Value token is here");
                // Extract the parameter title (assuming it's a string)
                int length = jsonTokens[i + 1].end - jsonTokens[i + 1].start;
                strncpy(parameterTitle, jsonString + jsonTokens[i + 1].start, length);
                parameterTitle[length] = '\0'; // Null-terminate the string
            }

        }
       
        } while (false);

#if dEXAMPLE_WITH_DYNAMIC_ALLOC == true
    // Check if the malloc has done correctly
    if (jsonTokens != NULL)
    {
        free(jsonTokens);
    }
#endif

    // Print the results
    printf("\r\n");
    printf("\r\n===== Results =====");
    printf("\r\n Parameter : %s", parameterTitle);
    printf("\r\n Configured Value: %d", configuredValue);
    
    // if (test)
    // {
    //     printf("\r\nTest: true");
    // }

    // Avoids console window terminate.
    // getch();

    return 0;
}

/** @} DOXYGEN GROUP TAG END OF FILE */