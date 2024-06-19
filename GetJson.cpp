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

string jsonData(string url) {
    CURL* curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    long httpCode(0);
    unique_ptr<string> httpData(new string());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());

    curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup(curl);

    if (httpCode == 200)
        return *httpData.get();
    else
        return "";
}



int main()
{
    string dificulltyLevel = "";
    string testSubject = "";
    string uniqueToken = "";

    const string url("https://opentdb.com/api.php?amount=5&token=" + uniqueToken);

    string thisRoundQuestions = jsonData(url);
    
    if (thisRoundQuestions != "") {
        json js = json::parse(thisRoundQuestions);

        if (js["response_code"] == 0) {
            json res = js["results"];

            for (int i = 0; i < 5; i++) {
                cout << res[i] << '\n';
            }
        }
    }
    else {
        cout << "There was an error!";
    }

    return 0;
}
