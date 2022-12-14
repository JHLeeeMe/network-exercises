#include <iostream>
#include "curl/curl.h"


int main(void)
{
    CURL* curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.example.com/");
        res = curl_easy_perform(curl);

        if(res == CURLE_OK)
        {
            char* ct;
            /* ask for the content-type */
            res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);

            if((res == CURLE_OK) && ct)
            {
                printf("We received Content-Type: %s\n", ct);
            }
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    return 0;
}
