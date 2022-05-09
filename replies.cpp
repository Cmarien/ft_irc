#include <iostream>

// 0xx REPLIES
std::string RPL_WELCOME(std::string prefix){
    return ":Welcome to the IRC Network " + prefix;
}
std::string RPL_YOURHOST(std::string servername, std::string version){
    return ":Your host is " + servername + ", running version " + version;
}
std::string RPL_CREATED(std::string date){
    return ":This server was created " + date;
}
std::string RPL_MYINFO(std::string servername, std::string version, std::string usermodes, std::string channelmodes){
    return servername + " " + version + " " + usermodes + " " + channelmodes;
}
std::string RPL_BOUNCE(std::string server, std::string port){
    return ":Try server " + server + ", port " + port;
}

// 2xx REPLIES
std::string RPL_TRACELINK(std::string version, std::string dest, std::string server){
    return "Link " + version + " " + dest + " " + server;
}
std::string RPL_TRACECONNECTING(std::string class_, std::string server){
    return "Try. " + class_ + " " + server;
}
std::string RPL_TRACEHANDSHAKE(std::string class_, std::string server){
    return "H.S. " + class_ + " " + server;
}
std::string RPL_TRACEUNKNOWN(std::string class_, std::string ip){
    return "???? " + class_ + " " + ip;
}
std::string RPL_TRACEOPERATOR(std::string class_, std::string nick){
    return "Oper " + class_ + " " + nick;
}
std::string RPL_TRACEUSER(std::string class_, std::string nick){
    return "User " + class_ + " " + nick;
}
std::string RPL_TRACESERVER(std::string class_, std::string int1, std::string int2, std::string server){
    return "Serv " + class_ + " " + int1 + "S " + int2 + "C " + server;
}
std::string RPL_TRACENEWTYPE(std::string newtype, std::string client){
    return newtype + " 0 " + client;
}
std::string RPL_TRACELOG(std::string logfile, std::string debug){
    return "FILE " + logfile + " " + debug;
}
std::string RPL_STATSLINKINFO(std::string linkname, std::string sendq, std::string smessage, std::string sbytes, std::string rmessage, std::string rbytes, std::string time){
    return linkname + " " + sendq + " " + smessage + " " + sbytes + " " + rmessage + " " + rbytes + " " + time;
}
std::string RPL_STATSCOMMANDS(std::string command, std::string count){
    return command + " " + count;
}
std::string RPL_STATSCLINE(std::string host, std::string name, std::string port, std::string class_){
    return "C " + host + " * " + name + " " + port + " " + class_;
}
std::string RPL_STATSNLINE(std::string host, std::string name, std::string port, std::string class_){
    return "N " + host + " * " + name + " " + port + " " + class_;
}
std::string RPL_STATSILINE(std::string host, std::string port, std::string class_){
    return "I " + host + " * " + host + " " + port + " " + class_;
}
std::string RPL_STATSKLINE(std::string host, std::string username, std::string port, std::string class_){
    return "K " + host + " * " + username + " " + port + " " + class_;
}
std::string RPL_STATSYLINE(std::string class_, std::string ping_freq, std::string connect_freq, std::string max_sendq){
    return "Y " + class_ + " " + ping_freq + " " + connect_freq + " " + max_sendq;
}
std::string RPL_ENDOFSTATS(std::string stats_letter){
    return stats_letter + " :End of /STATS report";
}
std::string RPL_STATSLLINE(std::string hostmask, std::string servername, std::string maxdepth){
    return "L " + hostmask + " * " + servername + " " + maxdepth;
}
std::string RPL_STATSUPTIME(){
    return ":Server Up %d days %d:%02d:%02d";
}
std::string RPL_STATSOLINE(std::string hostmask, std::string name){
    return "O " + hostmask + " * " + name;
}
std::string RPL_STATSHLINE(std::string hostmask, std::string servername){
    return "H " + hostmask + " * " + servername;
}
std::string RPL_UMODEIS(std::string user_mode){
    return user_mode;
}
std::string RPL_LUSERCLIENT(std::string int1, std::string int2, std::string int3){
    return ":There are " + int1 + " users and " + int2 + " invisible on " + int3 + " servers";
}
std::string RPL_LUSEROP(std::string int_){
    return int_ + " :operator(s) online";
}
std::string RPL_LUSERUNKNOWN(std::string int_){
    return int_ + " :unknown connection(s)";
}
std::string RPL_LUSERCHANNELS(std::string int_){
    return int_ + " :channels formed";
}
std::string RPL_LUSERME(std::string int1, std::string int2){
    return ":I have " + int1 + " clients and " + int2 + " servers";
}
std::string RPL_ADMINME(std::string server){
    return server + " :Administrative info";
}
std::string RPL_ADMINLOC1(std::string admin_info){
    return ":" + admin_info;
}
std::string RPL_ADMINLOC2(std::string admin_info){
    return ":" + admin_info;
}
std::string RPL_ADMINEMAIL(std::string admin_info){
    return ":" + admin_info;
}

//3xx REPLIES
std::string RPL_NONE(){
    return "";
}
std::string RPL_USERHOST(std::string replies){
    return ":" + replies;
}
std::string RPL_ISON(std::string nicks){
    return ":" + nicks;
}
std::string RPL_AWAY(std::string nick, std::string message){
    return nick + " :" + message;
}
std::string RPL_UNAWAY(){
    return ":You are no longer marked as being away";
}
std::string RPL_NOWAWAY(){
    return ":You have been marked as being away";
}
std::string RPL_WHOISUSER(std::string nick, std::string user, std::string host, std::string realname){
    return nick + " " + user + " " + host + " * :" + realname;
}
std::string RPL_WHOISSERVER(std::string nick, std::string server, std::string server_info){
    return nick + " " + server + " :" + server_info;
}
std::string RPL_WHOISOPERATOR(std::string nick){
    return nick + " :is an IRC operator";
}
std::string RPL_WHOISIDLE(std::string nick, std::string int_){
    return nick + " " + int_ + " :seconds idle";
}
std::string RPL_ENDOFWHOIS(std::string nick){
    return nick + " :End of /WHOIS list";
}
std::string RPL_WHOISCHANNELS(std::string nick, std::string channels){
    return nick + " :" + channels;
}
std::string RPL_WHOWASUSER(std::string nick, std::string user, std::string host, std::string real_name){
    return nick + " " + user + " " + host + " * :" + real_name;
}
std::string RPL_ENDOFWHOWAS(std::string nick){
    return nick + " :End of WHOWAS";
}
std::string RPL_LISTSTART(){
    return "Channel :Users Name";
}
std::string RPL_LIST(std::string channel, std::string visible, std::string topic){
    return channel + " " + visible + " :" + topic;
}
std::string RPL_LISTEND(){
    return ":End of /LIST";
}
std::string RPL_CHANNELMODEIS(std::string channel, std::string mode, std::string mode_param){
    return channel + " " + mode + " " + mode_param;
}
std::string RPL_NOTOPIC(std::string channel){
    return channel + " :No topic is iset";
}
std::string RPL_TOPIC(std::string channel, std::string topic){
    return channel + " :" + topic;
}
std::string RPL_INVITING(std::string channel, std::string nick){
    return channel + " " + nick;
}
std::string RPL_SUMMONING(std::string user){
    return user + " :Summoning user to IRC";
}
std::string RPL_VERSION(std::string version, std::string debug, std::string server, std::string comments){
    return version + "." + debug + " " + server + " :" + comments;
}
std::string RPL_WHOREPLY(std::string channel, std::string user, std::string host, std::string server, std::string nick){
    return channel + " " + user + " " + host + " " + server + " " + nick;
}
std::string RPL_ENDOFWHO(std::string name){
    return name + " :End of /WHO list";
}
std::string RPL_NAMREPLY(std::string mod, std::string channel, std::string nicks){
    return mod + " " + channel + " :" + nicks;
}
std::string RPL_ENDOFNAMES(std::string channel){
    return channel + " :End of /NAMES list";
}
std::string RPL_LINKS(std::string mask, std::string server, std::string hopcount, std::string serverinfo){
    return mask + " " + server + " :" + hopcount + " " + serverinfo;
}
std::string RPL_ENDOFLINKS(std::string mask){
    return mask + " :End of /LINKS list";
}
std::string RPL_BANLIST(std::string channel, std::string banid){
    return channel + " " + banid;
}
std::string RPL_ENDOFBANLIST(std::string channel){
    return channel + " :End of channel ban list";
}
std::string RPL_INFO(std::string str){
    return ":" + str;
}
std::string RPL_ENDOFINFO(){
    return ":End of /INFO list";
}
std::string RPL_MOTDSTART(std::string server){
    return ":- " + server + " Message of the day - ";
}
std::string RPL_MOTD(std::string text){
    return ":- " + text;
}
std::string RPL_ENDOFMOTD(){
    return ":End of /MOTD command";
}
std::string RPL_YOUREOPER(){
    return ":You are now an IRC operator";
}
std::string RPL_REHASHING(std::string config_file){
    return config_file + " :Rehashing";
}
std::string RPL_TIME(std::string server, std::string local_time){
    return server + " :" + local_time;
}
std::string RPL_USERSTART(){
    return ":UserID Terminal Host";
}
std::string RPL_USERS(){
    return ":%-8s %-9s %-8s";
}
std::string RPL_ENDOFUSERS(){
    return ":End of users";
}
std::string RPL_NOUSERS(){
    return ":Nobody logged in";
}
//4xx REPLIES (errors)
std::string ERR_NOSUCHNICK(std::string nickname){
    return nickname + " :No such nick/channel";
}
std::string ERR_NOSUCHSERVER(std::string server){
    return server + " :No such server";
}
std::string ERR_NOSUCHCHANNEL(std::string channel){
    return channel + " :No such channel";
}
std::string ERR_CANNOTSENDTOCHAN(std::string channel){
    return channel + " :Cannot send to channel";
}
std::string ERR_TOOMANYCHANNELS(std::string channel){
    return channel + " :You have joined too many";
}
std::string ERR_WASNOSUCHNICK(std::string nickname){
    return nickname + " :There was no such nickname";
}
std::string ERR_TOOMANYTARGETS(std::string target){
    return target + " :Duplicate recipients. No message delivered";
}
std::string ERR_NOORIGIN(){
    return ":No origin specified";
}
std::string ERR_NORECIPIENT(std::string command){
    return ":No recipient given (" + command + ")";
}
std::string ERR_NOTESTTOSEND(){
    return ":No text to send";
}
std::string ERR_NOTOPLEVEL(std::string mask){
    return mask + " :No toplevel domain specified";
}
std::string ERR_WILDTOPLEVEL(std::string mask){
    return mask + " :Wildcard in toplevel domain";
}
std::string ERR_UNKNOWNCOMMAND(std::string command){
    return command + " :Unknown command";
}
std::string ERR_NOMOTD(){
    return ":MOTD File is missing";
}
std::string ERR_NOADMININFO(std::string server){
    return server + " :No administrative info available";
}
std::string ERR_FILEERROR(std::string file_op, std::string file){
    return ":File error doing " + file_op + " on " + file;
}
std::string ERR_NONICKNAMEGIVEN(){
    return ":No nickname given";
}
std::string ERR_ERRONEUSNICKNAME(std::string nickname){
    return nickname + " :Erroneus nickname";
}
std::string ERR_NICKNAMEINUSE(std::string nickname){
    return nickname + " :Nickname is already in use";
}
std::string ERR_NICKCOLLISION(std::string nickname){
    return nickname + " :Nickname collision kill";
}
std::string ERR_USERNOTINCHANNEL(std::string nick, std::string channel){
    return nick + " " + channel + " :They aren't on that channel";
}
std::string ERR_NOTONCHANNEL(std::string channel){
    return channel + " :You're not on that channel";
}
std::string ERR_USERONCHANNEL(std::string user, std::string channel){
    return user + " " + channel + " :is already on channel";
}
std::string ERR_NOLOGIN(std::string user){
    return user + " :User not logged in";
}
std::string ERR_SUMMONDISABLED(){
    return ":SUMMON has been disabled";
}
std::string ERR_USERSDISABLED(){
    return ":USERS has been disabled";
}
std::string ERR_NOTREGISTERED(){
    return ":You have not registered";
}
std::string ERR_NEEDMOREPARAMS(std::string command){
    return command + " :Not enough parameters";
}
std::string ERR_ALREADYREGISTRED(){
    return ":You may not register";
}
std::string ERR_PASSWDMISMATCH(){
    return ":Password incorrect";
}
std::string ERR_YOUREBANNEDCREEP(){
    return ":You are banned from this server";
}
std::string ERR_KEYSET(std::string channel){
    return channel + " :Channel key already set";
}
std::string ERR_CHANNELISFULL(std::string channel){
    return channel + " :Cannot join channel (+1)";
}
std::string ERR_UNKNOWNMODE(std::string char_){
    return char_ + " :is unknown mode char to me";
}
std::string ERR_INVITEONLYCHAN(std::string channel){
    return channel + " :Cannot join channel (+i)";
}
std::string ERR_BANNEDFROMCHAN(std::string channel){
    return channel + " :Cannot join channel (+b)";
}
std::string ERR_BADCHANNELKEY(std::string channel){
    return channel + " :Cannot join channel (+k)";
}
std::string ERR_NOPRIVILEGES(){
    return ":Permission Denied- You're not an IRC operator";
}
std::string ERR_CHANOPRIVSNEEDED(std::string channel){
    return channel + " :You're not channel operator";
}
std::string ERR_CANTKILLSERVER(){
    return ":You cant kill a server!";
}
std::string ERR_NOOPERHOST(){
    return ":No O-lines for your host";
}
//5xx REPLIES
std::string ERR_UMODEUNKNOWNFLAG(){
    return ":Unknown MODE flag";
}
std::string ERR_USERSDONTMATCH(){
    return ":Cant change mode for other users";
}