#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>

#include "user.hpp"
#include "train.hpp"
#include "order.hpp"
#include "date.hpp"
#include "vector.hpp"

using std::string;

sjtu::vector<string> parser(const string &command_line) {
    sjtu::vector<string> words;
    string word = "";
    int len = command_line.length();
    for (int i = 0; i < len; i++) {
        if (command_line[i] == ' ') {
            while (i + 1 < len && command_line[i + 1] == ' ')
                i++;
            if (word != "")
                words.push_back(word);
            word = "";
            continue;
        }
        word += command_line[i];
    }
    if (word != "")
        words.push_back(word);
    return words;
}

int toint(const string &u) {
    int v = 0, len = u.length();
    for (int i = 0; i < len; i++)
        if (std::isdigit(u[i]))
            v = (v << 3) + (v << 1) + (u[i] ^ 48);
    return v;
}

sjtu::vector<string> divi(const string &u) {
    int len = u.length();
    string v = "";
    sjtu::vector<string> w;
    for (int i = 0; i < len; i++) {
        if (u[i] == '|'){
            if (v!="") w.push_back(v), v = "";
        }
        else
            v += u[i];
    }
    if (v != "")
        w.push_back(v);
    return w;
}

date today(const string &u) {
    return date(toint(u.substr(0, 2)), toint(u.substr(3, 2)), 0, 0);
}

date tomin(const string &u) {
    return date(6, 1, toint(u.substr(0, 2)), toint(u.substr(3, 2)));
}

string parse_command(const string &command_line) {
    std::cout<<command_line<<std::endl;
    sjtu::vector<string> words = parser(command_line);
    int z = words.size();
    if (words[0] == "add_user") {
        string username = "", password = "", realname = "", mailaddr = "";
        for (int i = 1; i < z; i += 2) {
            if (words[i] == "-u")
                username = words[i + 1];
            else if (words[i] == "-p")
                password = words[i + 1];
            else if (words[i] == "-n")
                realname = words[i + 1];
            else if (words[i] == "-m")
                mailaddr = words[i + 1];
        }
        if (username.length()>20) return "?????????????????????";
        if (password.length()<8) return "??????????????????";
        if (password.length()>30) return "??????????????????";
        if (realname.length()>20) return "?????????????????????????????????[??????]";
        if (mailaddr.length()>30) return "??????????????????";
        return userManagement.add_user(username, password, realname, mailaddr);
    }
    // std::cout<<command_line<<std::endl;
    if (words[0] == "login") {
        string username = "", password = "";
        for (int i = 1; i < z; i += 2) {
            if (words[i] == "-u")
                username = words[i + 1];
            else if (words[i] == "-p")
                password = words[i + 1];
        }
        if (username.length()>20) return "?????????????????????";
        if (password.length()<8) return "??????????????????";
        if (password.length()>30) return "??????????????????";
        return userManagement.login(username, password);
    }
    if (words[0] == "logout") {
        string username = words[1],password=words[2];
        if (username.length()>20) return "?????????????????????";
        if (password.length()<8) return "??????????????????";
        if (password.length()>30) return "??????????????????";
        return userManagement.logout(username,password);
    }
    // if (words[1] == "query_profile") {
    //     string cur_username = "", username = "";
    //     for (int i = 2; i < z; i += 2) {
    //         if (words[i] == "-u")
    //             username = words[i + 1];
    //         else if (words[i] == "-c")
    //             cur_username = words[i + 1];
    //     }
    //     return words[0] + " " + userManagement.query_profile(cur_username, username);
    // }
    if (words[0] == "modify_profile") {
        string password_pre = "", username = "", password = "", realname = "", mailaddr = "";
        int privilege = -1;
        for (int i = 1; i < z; i += 2) {
            if (words[i] == "-u")
                username = words[i + 1];
            else if (words[i] == "-p")
                password = words[i + 1];
            else if (words[i] == "-n")
                realname = words[i + 1];
            else if (words[i] == "-m")
                mailaddr = words[i + 1];
            else if (words[i] == "-g")
                password_pre = words[i + 1];
        }
        if (username.length()>20) return "?????????????????????";
        if (password.length()<8) return "??????????????????";
        if (password.length()>30) return "??????????????????";
        if (realname.length()>20) return "?????????????????????????????????[??????]???";
        if (mailaddr.length()>30) return "??????????????????";
        return userManagement.modify_profile(username, password, realname, mailaddr, password_pre);
    }
    if (words[0] == "add_train") {
        // std::cout<<"??"<<std::endl;
        string trainid = words[1], type = words[11];
        if (trainid.length()>20) return "??????id????????????~";
        sjtu::vector<string> station_name,u;
        int station_num = toint(words[3]), seat_num = toint(words[2]);
        
        if (station_num<2) return "???????????????????????????????????????";
        
        sjtu::vector<int> prices, travel_time, stopover_time;
        date start_time=tomin(words[10]), start_sale, end_sale;
        
        if (words[4]=="June") start_sale.month=6;
        if (words[4]=="July") start_sale.month=7;
        if (words[4]=="August") start_sale.month=8;
        start_sale.day=toint(words[5]);
        
        if (words[7]=="June") end_sale.month=6;
        if (words[7]=="July") end_sale.month=7;
        if (words[7]=="August") end_sale.month=8;
        end_sale.day=toint(words[8]);
        
        u=divi(words[12]);int uz=u.size();
        if (uz!=station_num) return "??????????????????~";
        for (int i=0;i<uz;i++)
            if (u[i].length()>40) return "?????????????????????????????????...";
        station_name=u;
        
        u=divi(words[13]);uz=u.size();
        if (uz!=station_num-1) return "???????????????????????????~";
        for (int i=0;i<uz;i++)
            travel_time.push_back(toint(u[i]));
        
        u=divi(words[14]);
        uz=u.size();
        if (uz!=station_num-1) return "??????????????????????????????~";
        for (int i=0;i<uz;i++)
            prices.push_back(toint(u[i]));
        
        u=divi(words[15]);
        uz=u.size();
        if (uz!=station_num-2) return "???????????????????????????~";
        for (int i=0;i<uz;i++)
            stopover_time.push_back(toint(u[i]));
        
        return trainManagement.add_train(trainid, station_num,
                                         seat_num, station_name, prices,
                                         start_time, travel_time, stopover_time,
                                         start_sale, end_sale, type);
    }
    if (words[0] == "delete_train") {
        string trainid = words[1];
        if (trainid.length()>20) return "??????id????????????~";
        return trainManagement.delete_train(trainid);
    }
    if (words[0] == "release_train") {
        string trainid = words[1];
        if (trainid.length()>20) return "??????id????????????~";
        return trainManagement.release_train(trainid);
    }
    if (words[0] == "query_train") {
        string trainid = words[1];
        if (trainid.length()>20) return "??????id????????????~";
        date depart;
        if (words[2]=="June") depart.month=6;
        if (words[2]=="July") depart.month=7;
        if (words[2]=="August") depart.month=8;
        depart.day=toint(words[3]);
        return trainManagement.query_train(trainid, depart);
    }
    if (words[0] == "query_ticket") {
        string start_station = words[2], end_station = words[4], compare = words[6];
        if (start_station.length()>40) return "??????????????????????????????~";
        if (end_station.length()>40) return "??????????????????????????????~";
        date depart;
        if (words[8]=="June") depart.month=6;
        if (words[8]=="July") depart.month=7;
        if (words[8]=="August") depart.month=8;
        depart.day=toint(words[9]);
        return trainManagement.query_ticket(start_station, end_station, depart, compare == "cost");
    }
    if (words[0] == "query_transfer") {
        string start_station = words[2], end_station = words[4], compare = words[6];
        if (start_station.length()>40) return "??????????????????????????????~";
        if (end_station.length()>40) return "??????????????????????????????~";
        date depart;
        if (words[8]=="June") depart.month=6;
        if (words[8]=="July") depart.month=7;
        if (words[8]=="August") depart.month=8;
        depart.day=toint(words[9]);
        return trainManagement.query_transfer(start_station, end_station, depart, compare == "cost");
    }
    if (words[0] == "buy_ticket") {
        string username = words[1], trainid = words[2], start = words[3], end = words[4], will = "false";
        if (start.length()>40) return "??????????????????????????????~";
        if (end.length()>40) return "??????????????????????????????~";
        if (username.length()>20) return "?????????????????????~";
        if (trainid.length()>20) return "??????id????????????~";
        int ticket_num=toint(words[5]);
        date start_date;
        if (words[6]=="June") start_date.month=6;
        if (words[6]=="July") start_date.month=7;
        if (words[6]=="August") start_date.month=8;
        start_date.day=toint(words[7]);
        if (z>9) will="true";
        return orderManagement.buy_ticket(username, trainid, start_date, ticket_num,
                                          start, end, will == "true");
    }
    if (words[0] == "query_order") {
        string username = words[1],password=words[2];
        if (username.length()>20) return "?????????????????????";
        if (password.length()<8) return "??????????????????";
        if (password.length()>30) return "??????????????????";
        return orderManagement.query_order(username,password);
    }
    if (words[0] == "refund_ticket") {
        string username = words[1];
        if (username.length()>20) return "?????????????????????";
        int num = toint(words[2]);
        return orderManagement.refund_ticket(username, num);
    }
    // if (words[1] == "rollback") {
    //     int now = toint(words[0]), pre = toint(words[3]);
    //     if (now < pre) return "-1\n";
    //     userManagement.rollback(pre);
    //     trainManagement.rollback(pre);
    //     orderManagement.rollback(pre);
    //     return words[0] + " 0\n";
    // }
    // if (words[1] == "clean") {
    //     userManagement.clean();
    //     trainManagement.clean();
    //     orderManagement.clean();
    //     return words[0] + " 0\n";
    // }
    // if (words[1] == "exit") {
    //     std::cout << words[0] << " bye\n";
    //     exit(0);
    // }
    return "fxxk";
}

#endif
