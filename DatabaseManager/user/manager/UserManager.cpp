//
// Created by 李志豪 on 9/26/15.
//

#include "UserManager.h"

UserManager::UserManager() {
    cppdb::session sql(db_connection);
    this->sql_session = sql;
}

bool UserManager::save_user(std::shared_ptr<User> user) {
    try {
        cppdb::statement stat;
        stat = this->sql_session << "INSERT INTO user (username, password, status, usercode) values (?, ?, ?, ?)" <<
               user->get_username() << user->get_password() << user->get_status() << user->get_usercode();
        stat.exec();
        return true;
    }catch (std::exception const &e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return false;
    }
}

std::shared_ptr<User> UserManager::get_user(std::shared_ptr<User> user, int condition) {
    std::shared_ptr<User>user_from_db(new User());
    cppdb::result res;
    if (condition==0){
        res = this->sql_session << "SELECT * from user where usercode=?" << user->get_usercode();
    } else if(condition==1){
        res = this->sql_session << "SELECT * from user where username=?" << user->get_username();
    } else if(condition==2){
        res = this->sql_session << "SELECT * from user where username=? and password=?" << user->get_username() << user->get_password();
    } else if(condition==3){
        res = this->sql_session << "SELECT * from user where status=?" << user->get_status();
    }
    while (res.next()) {
        int id;
        int status;
        std::string username;
        std::string password;
        std::string usercode;
        res >> id >> username >> password >> usercode >> status;
        user_from_db->set_username(username);
        user_from_db->set_password(password);
        user_from_db->set_usercode(usercode);
        user_from_db->set_status(status);
        return user_from_db;
    }
    return user_from_db;
}

std::shared_ptr<std::vector<std::shared_ptr<User>>> UserManager::get_user(std::shared_ptr<User> user, int start_index, int length) {
    std::shared_ptr<std::vector<std::shared_ptr<User>>>user_list(new std::vector<std::shared_ptr<User>>());
    cppdb::result res;
    res = this->sql_session << "SELECT * from user where status=? order by id desc limit ?, ?" << user->get_status() << start_index << length;
    while (res.next()) {
        std::shared_ptr<User>user_from_db(new User());
        int id;
        int status;
        std::string username;
        std::string password;
        std::string usercode;
        res >> id >> username >> password >> usercode >> status;
        user_from_db->set_username(username);
        user_from_db->set_password(password);
        user_from_db->set_usercode(usercode);
        user_from_db->set_status(status);
        user_list->push_back(std::move(user_from_db));
    }
    return user_list;
}

int UserManager::get_user_count(int status) {
    cppdb::result res;
    res = this->sql_session << "SELECT count(*) from user where status=?" << status;
    while (res.next()) {
        int count;
        res >> count;
        return count;
    }
    return 0;
}

bool UserManager::set_user_status(std::string usercode, int status) {
    try {
        cppdb::statement stat;
        stat = this->sql_session << "update user set status=? where usercode=?" << status << usercode;
        stat.exec();
        return true;
    }catch (std::exception const &e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return false;
    }
}
