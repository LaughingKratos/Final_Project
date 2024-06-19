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

vector<string> Subjects;
vector<int> SubjctsId;

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

void PvPRound() {
    string dificulltyLevel = "";
    string testSubject = "";
    string uniqueToken = "";

    const string url("https://opentdb.com/api.php?amount=5");

    vector<string> questions;
    vector<vector<string>> wrongAnswers;
    vector<string> trueAnswer;

    string thisRoundItems = jsonData(url);

    if (thisRoundItems != "") {
        json js = json::parse(thisRoundItems);

        if (js["response_code"] == 0) {
            json res = js["results"];

            for (int i = 0; i < 5; i++) {
                questions.push_back(res[i]["question"]);
                trueAnswer.push_back(res[i]["correct_answer"]);
                
                vector<string> wa;
                json wajson = res[i]["incorrect_answers"];
                for (short j = 0; j < 3; j++) {
                    wa.push_back(wajson[j]);
                }
                wrongAnswers.push_back(wa);
            }
        }
    }
    else {
        cout << "There was an error!";
    }
}

void GetSubjects() {
    const string url("https://opentdb.com/api_category.php");
    json subs = json::parse(jsonData(url));

    for (int i = 0; i < subs["trivia_categories"].size(); i++) {
        Subjects.push_back(subs["trivia_categories"][i]["name"]);
        SubjctsId.push_back(subs["trivia_categories"][i]["id"]);
    }
}

int main()
{
    GetSubjects();
    PvPRound();

    return 0;
}
