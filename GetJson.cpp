#include <iostream>
#include <string>
#include <vector>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

namespace
{
    std::size_t callback(
        const char* in,
        std::size_t size,
        std::size_t num,
        std::string* out)
    {
        const std::size_t totalBytes(size * num);
        out->append(in, totalBytes);
        return totalBytes;
    }
}

int main()
{
    string dificulltyLevel = "";
    string testSubject = "";

    // CURL interaction section
    const std::string url("https://opentdb.com/api.php?amount=5");

    CURL* curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    long httpCode(0);
    std::unique_ptr<std::string> httpData(new std::string());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());

    curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup(curl);
    // Results are already stored in httpData, an ending to curl interaction


    //modifing json into devs-defining variables
    if (httpCode == 200)
    {
        json js = json::parse(*httpData.get());

        if (js["response_code"] == 0) {
            json res = js["results"];

            for (int i = 0; i < 5; i++) {
                cout << res[i]<<'\n';
            }
        }
    }
    else
    {
        cout << "Couldn't GET from " << url << " - exiting" << httpCode << std::endl;
        return 1;
    }

    return 0;
}
