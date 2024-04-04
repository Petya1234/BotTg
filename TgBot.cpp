#include <iostream>
#include <tgbot/tgbot.h>
#include <cpr/cpr.h>
#include <string>
#include <nlohmann/json.hpp>



using json = nlohmann::json;
using namespace std;
using namespace TgBot;

vector<string> responseCodeForces(vector<string>& arr) {
    string response;
    cpr::Response r = cpr::Get(cpr::Url{ "https://codeforces.com/api/contest.list?gym=false" },
        cpr::Parameters{ {"key", "e7c0254644e15459ed8fc771561ecbb64a46c62c"} });

    response = r.text;
    json responseJson = json::parse(response);
    json object = responseJson["result"];
    for (int i = 0; i < object.size(); i++) {
        if (object[i]["phase"] == "BEFORE") {
            arr.push_back(object[i]["name"]);
        }
        else {
            break;
        }
    }
    return arr;
}


int main() {
    vector<string> codeForcesResponse;

    InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
    vector<InlineKeyboardButton::Ptr> row0;
    InlineKeyboardButton::Ptr checkButton(new InlineKeyboardButton);
    checkButton->text = "Get events";
    checkButton->callbackData = "getÑontests";
    row0.push_back(checkButton);

    keyboard->inlineKeyboard.push_back(row0);
    responseCodeForces(codeForcesResponse);
    for (int i = 0; i < codeForcesResponse.size(); i++) {
        cout << codeForcesResponse[i] << endl;
    }
    Bot bot("7023822643:AAEwT8kBOO01UNLrBdXEIDQCSZUYcVUHen4");
    bot.getEvents().onCommand("start", [&bot, &keyboard](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Hi! It is a contests bot \nHere you can get info about programming contests", false, 0, keyboard);
        });

    bot.getEvents().onCallbackQuery([&bot, &keyboard](CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "getContests")) {
            string response = "Here is the closest contests";
            bot.getApi().sendMessage(query->message->chat->id, response, false, 0, keyboard, "Markdown");
        }
        });
    signal(SIGINT, [](int s) {
        printf("SIGINT got\n");
        exit(0);
        });

    bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {
        printf("User wrote %s\n", message->text.c_str());
        if (StringTools::startsWith(message->text, "/start")) {
            return;
        }
        bot.getApi().sendMessage(message->chat->id, "Your message is: " + message->text);
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