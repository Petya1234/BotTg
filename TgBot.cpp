#include <iostream>
#include <tgbot/tgbot.h>
#include <cpr/cpr.h>
#include <string>
#include <nlohmann/json.hpp>



using json = nlohmann::json;
using namespace std;
using namespace TgBot;

vector<string> codeForcesResponse;
string dailyLinkLeetCode = "";

void responseCodeForces(vector<string>& arr) {
    string response;
    cpr::Response r = cpr::Get(cpr::Url{ "https://codeforces.com/api/contest.list?gym=false" });

    response = r.text;
    json responseJson = json::parse(response);
    json object = responseJson["result"];
    for (int i = 0; i < object.size(); i++) {
        if (object[i]["phase"] == "BEFORE") {
            arr.push_back(object[i]["name"]);
        }
        else {
            return;
        }
    }
}

string responseLeetCode() {
    string response;
    cpr::Response r = cpr::Get(cpr::Url{ "https://alfa-leetcode-api.onrender.com/daily" });

    response = r.text;
    json responseJson = json::parse(response);
    json link = responseJson["questionLink"];
    return link;
}


int main() {

    responseLeetCode();
    InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
    vector<InlineKeyboardButton::Ptr> row0;
    InlineKeyboardButton::Ptr checkButton(new InlineKeyboardButton);
    checkButton->text = "Get contests";
    checkButton->callbackData = "Gtc";
    row0.push_back(checkButton);
    keyboard->inlineKeyboard.push_back(row0);


    Bot bot("7023822643:AAEwT8kBOO01UNLrBdXEIDQCSZUYcVUHen4");

    bot.getEvents().onCommand("start", [&bot, &keyboard](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Hi!", false, 0, keyboard);
        });

    bot.getEvents().onCallbackQuery([&bot, &keyboard](CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "Gtc")) {
            dailyLinkLeetCode = responseLeetCode();
            responseCodeForces(codeForcesResponse);
            if (codeForcesResponse.size() > 0) {
                string response = "Codeforces:";
                bot.getApi().sendMessage(query->message->chat->id, response, false, 0);
                for (int i = 0; i < codeForcesResponse.size(); i++) {
                    bot.getApi().sendMessage(query->message->chat->id, codeForcesResponse[i], false, 0);
                }
                response = "To get this competitions follow by link: ";
                bot.getApi().sendMessage(query->message->chat->id, response, false, 0);
                string link = "https://codeforces.com/contests";
                bot.getApi().sendMessage(query->message->chat->id, link, false, 0);
            }
            string response = "Daily Leetcode:";
            if (dailyLinkLeetCode != "") {
                bot.getApi().sendMessage(query->message->chat->id, response, false, 0);
                bot.getApi().sendMessage(query->message->chat->id, dailyLinkLeetCode, false, 0);
            }
            codeForcesResponse.clear();
        }
        
        });
    

    try {

        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();

        TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    }
    catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }
    return 0;
}