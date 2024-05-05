#include <iostream>
#include <format>
#include <tgbot/tgbot.h>
#include <cpr/cpr.h>
#include <string>
#include <nlohmann/json.hpp>


using json = nlohmann::json;
using namespace std;
using namespace TgBot;

vector<string> codeForcesResponse;
vector<string> problems;
string dailyLinkLeetCode = "";


void getProblemsByTags(vector<string>& problems,vector<string> arr)
{
   string response;
   string req = "";
   for (int i = 0; i < arr.size(); i++)
   {
       req += arr[i] + ";";
   }
   cpr::Response r = cpr::Get(cpr::Url{format("https://codeforces.com/api/problemset.problems?tags={}", req)});
   response = r.text;
   json responseJson = json::parse(response);
   json object = responseJson["result"]["problems"];
   for (int i = 0; i < object.size(); i++) {
       if(to_string(object[i]["index"]).length() == 4){
           problems.push_back(to_string(object[i]["contestId"]) + "/" + to_string(object[i]["index"])[1]+to_string(object[i]["index"])[2]);
       }
       else
       {
           problems.push_back(to_string(object[i]["contestId"]) + "/" + to_string(object[i]["index"])[1]);
       }
   }
}

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


int main() {
    vector<string> themes;


    InlineKeyboardMarkup::Ptr keyboardMain(new InlineKeyboardMarkup);
    vector<InlineKeyboardButton::Ptr> row0, row1;
 
    InlineKeyboardButton::Ptr checkButtonCF(new InlineKeyboardButton);
    InlineKeyboardButton::Ptr checkButtonLeetcode(new InlineKeyboardButton);
    InlineKeyboardButton::Ptr checkButtonGetMaterials(new InlineKeyboardButton);
    checkButtonCF->text = "Get availible contests from CF or CF archive";
    checkButtonCF->callbackData = "GtCfAr";
    row0.push_back(checkButtonCF);
    

    checkButtonGetMaterials->text = "Get materials";
    checkButtonGetMaterials->callbackData = "Gtm";
    row1.push_back(checkButtonGetMaterials);
    keyboardMain->inlineKeyboard.push_back(row0);
    keyboardMain->inlineKeyboard.push_back(row1);


    InlineKeyboardMarkup::Ptr keyboardCF(new InlineKeyboardMarkup);
    vector<InlineKeyboardButton::Ptr> row0Cf, row1Cf;
 
    InlineKeyboardButton::Ptr checkButtonGetCont(new InlineKeyboardButton);
    InlineKeyboardButton::Ptr checkButtonCfZProblems(new InlineKeyboardButton);
    checkButtonGetCont->text = "Get availible contests from CF";
    checkButtonGetCont->callbackData = "Gtc";
    row0Cf.push_back(checkButtonGetCont);
    
    checkButtonCfZProblems->text = "Get problems by tags";
    checkButtonCfZProblems->callbackData = "GtcAr";
    row1Cf.push_back(checkButtonCfZProblems);

    keyboardCF->inlineKeyboard.push_back(row0Cf);
    keyboardCF->inlineKeyboard.push_back(row1Cf);



    InlineKeyboardMarkup::Ptr keyboardCFTagsAndConfirm(new InlineKeyboardMarkup);
    vector<InlineKeyboardButton::Ptr> r2Pointers, rDsu, rGraphs, rDp, rGames, rBinarySearch, rDS, rConfirm;
 
    InlineKeyboardButton::Ptr Buttonr2Pointers(new InlineKeyboardButton);
    InlineKeyboardButton::Ptr ButtonrDsu(new InlineKeyboardButton);
    InlineKeyboardButton::Ptr ButtonrGraphs(new InlineKeyboardButton);
    InlineKeyboardButton::Ptr ButtonrDp(new InlineKeyboardButton);
    InlineKeyboardButton::Ptr ButtonrGames(new InlineKeyboardButton);
    InlineKeyboardButton::Ptr ButtonrBinarySearch(new InlineKeyboardButton);
    InlineKeyboardButton::Ptr ButtonrDS(new InlineKeyboardButton);
    InlineKeyboardButton::Ptr ButtonrConfirm(new InlineKeyboardButton);
    Buttonr2Pointers->text = "2-pointers";
    Buttonr2Pointers->callbackData = "2Pointers";
    ButtonrDsu->text = "DSU";
    ButtonrDsu->callbackData = "Dsu";
    ButtonrGraphs->text = "Graphs";
    ButtonrGraphs->callbackData = "Graphs";
    ButtonrDp->text = "Dynamic programming";
    ButtonrDp->callbackData = "DP";
    ButtonrGames->text = "Games theory";
    ButtonrGames->callbackData = "Gt";
    ButtonrBinarySearch->text = "Binary search";
    ButtonrBinarySearch->callbackData = "BinS";
    ButtonrDS->text = "Data structures";
    ButtonrDS->callbackData = "DS";
    ButtonrConfirm->text = "Confirm selection";
    ButtonrConfirm->callbackData = "Conf";

    r2Pointers.push_back(Buttonr2Pointers);
    rDsu.push_back(ButtonrDsu);
    rGraphs.push_back(ButtonrGraphs);
    rDp.push_back(ButtonrDp);
    rGames.push_back(ButtonrGames);
    rBinarySearch.push_back(ButtonrBinarySearch);
    rDS.push_back(ButtonrDS);
    rConfirm.push_back(ButtonrConfirm);
    

    keyboardCFTagsAndConfirm->inlineKeyboard.push_back(r2Pointers);
    keyboardCFTagsAndConfirm->inlineKeyboard.push_back(rDsu);
    keyboardCFTagsAndConfirm->inlineKeyboard.push_back(rGraphs);
    keyboardCFTagsAndConfirm->inlineKeyboard.push_back(rDp);
    keyboardCFTagsAndConfirm->inlineKeyboard.push_back(rGames);
    keyboardCFTagsAndConfirm->inlineKeyboard.push_back(rBinarySearch);
    keyboardCFTagsAndConfirm->inlineKeyboard.push_back(rDS);
    keyboardCFTagsAndConfirm->inlineKeyboard.push_back(rConfirm);

    Bot bot("7023822643:AAEwT8kBOO01UNLrBdXEIDQCSZUYcVUHen4");

    bot.getEvents().onCommand("start", [&bot, &keyboardMain](Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "Hi!\nIts a bot for beginners in competitive programming\nHere you can fast get list of CF contests\nLettcode daily task and materials that can help you improve you skills", false, 0, keyboardMain);
        });


    bot.getEvents().onCallbackQuery([&bot, &keyboardMain](CallbackQuery::Ptr query) {
        if (query->data == "Gtc") {
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
                bot.getApi().sendMessage(query->message->chat->id, link, false, 0, keyboardMain);
            }
            codeForcesResponse.clear();
        }
    });

    bot.getEvents().onCallbackQuery([&bot, &keyboardMain, &keyboardCFTagsAndConfirm](CallbackQuery::Ptr query) {
        if (query->data == "GtcAr") {
            string response = "Select themes:";
            bot.getApi().sendMessage(query->message->chat->id, response, false, 0, keyboardCFTagsAndConfirm);
                 
        }
    });
    bot.getEvents().onCallbackQuery([&bot, &keyboardMain, &keyboardCF](CallbackQuery::Ptr query) {
        if (StringTools::startsWith(query->data, "GtCfAr")) {
            string response = "Please select:";
                bot.getApi().sendMessage(query->message->chat->id, response, false, 0, keyboardCF);
            
        }

        if (StringTools::startsWith(query->data, "Gtm")) {
            string response;
            response = "There is my collect with useful materials about competitive programming";
            bot.getApi().sendMessage(query->message->chat->id, response, false, 0);
            response = "Useful site for newbees:";
            bot.getApi().sendMessage(query->message->chat->id, response, false, 0);
            response = "https://acmp.ru/";
            bot.getApi().sendMessage(query->message->chat->id, response, false, 0);
            response = "Some algorithms sites, channels, materials:";
            bot.getApi().sendMessage(query->message->chat->id, response, false, 0);
            response = "https://ru.algorithmica.org/";
            bot.getApi().sendMessage(query->message->chat->id, response, false, 0);
            response = "https://peltorator.ru/cp_book.pdf";
            bot.getApi().sendMessage(query->message->chat->id, response, false, 0);
            response = "http://e-maxx.ru/algo/";
            bot.getApi().sendMessage(query->message->chat->id, response, false, 0);
            response = "https://www.youtube.com/@pavelmavrin";
            bot.getApi().sendMessage(query->message->chat->id, response, false, 0);
            response = "Stankevich site:";
            bot.getApi().sendMessage(query->message->chat->id, response, false, 0);
            response = "https://neerc.ifmo.ru/school/information/index.html";
            bot.getApi().sendMessage(query->message->chat->id, response, false, 0, keyboardMain);
            }
        });

    bot.getEvents().onCallbackQuery([&bot, &themes](CallbackQuery::Ptr query)
        {
        if (query->data == "2Pointers")
        {
            themes.push_back("two%20pointers");
        }
        });
    bot.getEvents().onCallbackQuery([&bot, &themes](CallbackQuery::Ptr query)
    {
    if (query->data == "Dsu")
    {
         themes.push_back("dsu");
    }
        });
        bot.getEvents().onCallbackQuery([&bot, &themes](CallbackQuery::Ptr query)
        {
        if (query->data == "Graphs")
        {
            themes.push_back("graphs");
        }
        });
        bot.getEvents().onCallbackQuery([&bot, &themes](CallbackQuery::Ptr query)
        {
        if (query->data == "DP")
        {
            themes.push_back("dp");
        }
        });
        bot.getEvents().onCallbackQuery([&bot, &themes](CallbackQuery::Ptr query)
        {
        if (query->data == "Gt")
        {
            themes.push_back("games");
        }
        });
        bot.getEvents().onCallbackQuery([&bot, &themes](CallbackQuery::Ptr query)
        {
        if (query->data == "BinS")
        {
            themes.push_back("binary%20search");
        }
        });
        bot.getEvents().onCallbackQuery([&bot, &themes](CallbackQuery::Ptr query)
        {
        if (query->data == "DS")
        {
            themes.push_back("data%20structures");
        }
        });
        bot.getEvents().onCallbackQuery([&bot,&keyboardMain, &themes](CallbackQuery::Ptr query)
        {
        if (query->data == "Conf")
        {
           
            getProblemsByTags(problems,themes);
            
            if (problems.size() != 0)
            {
                if (problems.size() / 10 > 0)
                {
                string response;
                response = "10 problems based on your tags";
                bot.getApi().sendMessage(query->message->chat->id, response, false, 0);
                for (int i = 0; i < 10; i++) {
                    response = format("https://codeforces.com/problemset/problem/{}", problems[i]);
                    bot.getApi().sendMessage(query->message->chat->id, response, false, 0);
                }

                string req = "https://codeforces.com/problemset?tags=";
                for (int i = 0; i < themes.size(); i++)
                {
                    if (i == themes.size() - 1)
                    {
                        req += themes[i];
                    }
                    else
                    {
                        req += themes[i] + ",";
                    }
                }
                response = format("All {} problems are located here {}", problems.size(), req);
                bot.getApi().sendMessage(query->message->chat->id, response, false, 0, keyboardMain);
                }
            }
            problems.clear();
            themes.clear(); 
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