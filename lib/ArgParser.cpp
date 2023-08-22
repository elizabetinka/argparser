#include "ArgParser.h"

using namespace ArgumentParser;

ArgParser::ArgParser(const std::string& s)
    :parser_name{s}{}

bool ArgParser::Parse(const std::vector<std::string>& mass) {
    if (mass.size() == 0) {
        return true;
    }
    if (mass.size() == 1) {
        if (string_arguments.empty() && int_arguments.empty() && flag_arguments.empty()) {
            return true;
        }
        for (int i = 0; i < string_arguments.size(); i++) {
            if (string_arguments[i]->default_value == false) {
                return false;
            }
        }
        for (int i = 0; i < int_arguments.size(); i++) {
            if (int_arguments[i]->default_value == false) {
                return false;
            }
        }
        for (int i = 0; i < flag_arguments.size(); i++) {
            if (flag_arguments[i]->default_value == false) {
                return false;
            }
        }

        return true;
    }
    for (int i = 1; i < mass.size(); i++) {
        std::string value;
        if (mass[i][0] == '-') {
            if (mass[i].length()<=1){
                continue;
            }
            if (mass[i][1] == '-') {
                std::string current_param;

                size_t index=mass[i].find('=');
                if (index==std::string::npos){
                    current_param=mass[i].substr(2);

                    // обрабатываем аргументы без значений
                    if (having_help_argument == true) {
                        if (current_param == long_name_help_argument) {
                            use_have_argument = true;
                            std::cout << HelpDescription();
                            return true;
                        }
                    }

                    // не help, возможно flag
                    bool findflag = false;
                    for (int m = 0; m < flag_arguments.size(); m++) {
                        if (flag_arguments[m]->name == current_param) {
                            flag_arguments[m]->key = true;
                            for (int k = 0; k < flag_arguments[m]->partner_key.size(); k++) {
                                *(flag_arguments[m]->partner_key[k]) = true;
                            }
                            findflag = true;
                            break;
                        }
                    }

                    if (findflag == true) {
                        continue;
                    }
                    else{
                        return false;
                    }

                }
                else{
                    current_param=mass[i].substr(2,index-2);
                    if (index==mass[i].length()-1){
                        // неверный формат ввода
                        return false;
                    }
                    value=mass[i].substr(index+1);
                }

                for (int j = 0; j < string_arguments.size(); j++) {
                    if (string_arguments[j]->name == current_param) {
                        string_arguments[j]->key = value;
                        for (int m = 0; m < string_arguments[j]->partner_key.size(); m++) {
                            *(string_arguments[j]->partner_key[m]) = value;
                        }
                        break;
                    }
                }

                for (int j = 0; j < int_arguments.size(); j++) {
                    if (int_arguments[j]->name == current_param) {
                        if (int_arguments[j]->multi_value == false) {
                            int_arguments[j]->key = std::stoi(value);
                            for (int m = 0; m < int_arguments[j]->partner_key.size(); m++) {
                                *(int_arguments[j]->partner_key[m]) = std::stoi(value);
                            }
                        } else {
                            int_arguments[j]->keys.push_back(std::stoi(value));
                            for (int m = 0; m < int_arguments[j]->partner_key_multi.size(); m++) {
                                (*(int_arguments[j]->partner_key_multi[m])).push_back(std::stoi(value));
                            }
                        }
                        break;
                    }
                }
            } else {
                char current = mass[i][1];

                if (having_help_argument == true) {
                    if (current == short_name_help_argument) {
                        std::cout << HelpDescription();
                        return true;
                    }
                }

                bool findflag = false;
                for (int j = 0; j < flag_arguments.size(); j++) {
                    if (flag_arguments[j]->short_name == current) {
                        flag_arguments[j]->key = true;
                        for (int k = 0; k < flag_arguments[j]->partner_key.size(); k++) {
                            *(flag_arguments[j]->partner_key[k]) = true;
                        }
                        findflag = true;
                        break;
                    }
                }
                if (findflag == true) {
                    for (int l = 2; l < mass[i].length(); l++) {
                        for (int j = 0; j < flag_arguments.size(); j++) {
                            if (flag_arguments[j]->short_name == mass[i][l]) {
                                flag_arguments[j]->key = true;
                                for (int k = 0; k < flag_arguments[j]->partner_key.size(); k++) {
                                    *(flag_arguments[j]->partner_key[k]) = true;
                                }
                                break;
                            }
                        }
                    }
                    continue;
                }
                // введен неправильный параметр
                if (mass[i].length()==2 && findflag== false){
                    return false;
                }
                if (mass[i].length()>=3 && mass[i][2]!='='){
                    // неправильный формат ввода
                    return false;
                }
                if (mass[i].length()<4){
                    // неправильный формат ввода
                    return false;
                }

                value=mass[i].substr(3);

                for (int j = 0; j < string_arguments.size(); j++) {
                    if (string_arguments[j]->short_name == current) {
                        string_arguments[j]->key = value;
                        for (int m = 0; m < string_arguments[j]->partner_key.size(); m++) {
                            *(string_arguments[j]->partner_key[m]) = value;
                        }
                        break;
                    }
                }

                for (int j = 0; j < int_arguments.size(); j++) {
                    if (int_arguments[j]->short_name == current) {
                        if (int_arguments[j]->multi_value == false) {
                            int_arguments[j]->key = std::stoi(value);
                            for (int m = 0; m < int_arguments[j]->partner_key.size(); m++) {
                                *(int_arguments[j]->partner_key[m]) = std::stoi(value);
                            }
                        } else {
                            int_arguments[j]->keys.push_back(std::stoi(value));
                            for (int m = 0; m < int_arguments[j]->partner_key_multi.size(); m++) {
                                (*(int_arguments[j]->partner_key_multi[m])).push_back(std::stoi(value));
                            }
                        }


                        break;
                    }
                }
            }
            // свободные аргументы
        } else {
            for (int j = 0; j < int_arguments.size(); j++) {
                if (int_arguments[j]->positional == true) {
                    if (int_arguments[j]->multi_value == true) {
                        int_arguments[j]->keys.push_back(std::stoi(mass[i]));
                        for (int m = 0; m < int_arguments[j]->partner_key_multi.size(); m++) {
                            (*(int_arguments[j]->partner_key_multi[m])).push_back(std::stoi(mass[i]));
                        }
                    } else {
                        int_arguments[j]->key = std::stoi(mass[i]);
                        for (int m = 0; m < int_arguments[j]->partner_key.size(); m++) {
                            *(int_arguments[j]->partner_key[m]) = std::stoi(mass[i]);
                        }
                    }
                }
                break;
            }
        }
    }
    // проверка введено ли минимальное количество данных для MultiValues
    for (int i = 0; i < int_arguments.size(); i++) {
        if (int_arguments[i]->multi_value == true) {
            if (int_arguments[i]->keys.size() < int_arguments[i]->min_args_count) {
                return false;
            }
        }
    }
    for (int i = 0; i < string_arguments.size(); i++) {
        if (string_arguments[i]->multi_value == true) {
            if (string_arguments[i]->keys.size() < string_arguments[i]->min_args_count) {
                return false;
            }
        }
    }
    return true;


}

bool ArgParser::Parse(int argc, char** argv) {
    std::vector<std::string> parse;
    for (int i = 0; i < argc; i++) {
        parse.push_back(argv[i]);
    }
    return Parse(parse);
}

ArgParser::StringArgument& ArgParser::AddStringArgument(const std::string& longer, const std::string& description) {
    StringArgument* x = new StringArgument(longer);
    x->description_ = description;
    string_arguments.push_back(x);
    return *x;
}

ArgParser::StringArgument&
ArgParser::AddStringArgument(char shorter, const std::string& longer, const std::string& description) {
    StringArgument* x = new StringArgument(shorter, longer);
    x->description_ = description;
    string_arguments.push_back(x);
    return *x;
}

const std::string& ArgParser::GetStringValue(const std::string& s, size_t index) {
    for (int i = 0; i < string_arguments.size(); i++) {
        if (string_arguments[i]->name == s) {
            if (string_arguments[i]->multi_value == true) {
                return string_arguments[i]->keys[index];
            } else {
                return string_arguments[i]->key;
            }
        }
    }
    return "";
}

ArgParser::IntArgument& ArgParser::AddIntArgument(const std::string& longer, const std::string& description) {
    IntArgument* x = new IntArgument(longer);
    x->description_ = description;
    int_arguments.push_back(x);
    return *x;
}

ArgParser::IntArgument&
ArgParser::AddIntArgument(char shorter, const std::string& longer, const std::string& description) {
    IntArgument* x = new IntArgument(shorter, longer);
    x->description_ = description;
    int_arguments.push_back(x);
    return *x;
}

int ArgParser::GetIntValue(const std::string& s, size_t index) {
    for (int i = 0; i < int_arguments.size(); i++) {
        if (int_arguments[i]->name == s) {
            if (int_arguments[i]->multi_value == true) {
                return int_arguments[i]->keys[index];
            } else {
                return int_arguments[i]->key;
            }
        }
    }
    return 0;
}

ArgParser::FlagArgument& ArgParser::AddFlag(const std::string& longer, const std::string& description) {
    FlagArgument* x = new FlagArgument(longer);
    x->description_ = description;
    flag_arguments.push_back(x);
    return *x;
}

ArgParser::FlagArgument& ArgParser::AddFlag(char shorter, const std::string& longer, const std::string& description) {
    FlagArgument* x = new FlagArgument(shorter, longer);
    x->description_ = description;
    flag_arguments.push_back(x);
    return *x;
}

bool ArgParser::GetFlag(const std::string& s) {
    for (int i = 0; i < flag_arguments.size(); i++) {
        if (flag_arguments[i]->name == s) {
            return flag_arguments[i]->key;
        }
    }
    return false;
}

void ArgParser::AddHelp(char shorter, const std::string& longer, const std::string& description) {
    having_help_argument = true;
    long_name_help_argument = longer;
    short_name_help_argument = shorter;
    description_help_argument = description;
    return;
}

const std::string ArgParser::HelpDescription() {
    std::string ans = parser_name + '\n' + description_help_argument + '\n' + '\n';

    for (int i = 0; i < string_arguments.size(); i++) {
        ans = ans + '-' + string_arguments[i]->short_name + ", --" + string_arguments[i]->name +
              "=<string>,  " + string_arguments[i]->description_;
        if (string_arguments[i]->min_args_count != 0) {
            ans = ans + " [repeated, min args = " + std::to_string(string_arguments[i]->min_args_count) + "]\n";
        }
        if (string_arguments[i]->default_value == true) {
            ans = ans + "[default = " + string_arguments[i]->key + "]\n";
        }
    }

    for (int i = 0; i < int_arguments.size(); i++) {
        ans = ans + '-' + int_arguments[i]->short_name + ", --" + int_arguments[i]->name +
              "=<int>,  " + int_arguments[i]->description_;
        if (int_arguments[i]->min_args_count != 0) {
            ans = ans + " [repeated, min args = " + std::to_string(int_arguments[i]->min_args_count) + "]\n";
        }
        if (int_arguments[i]->default_value == true) {
            ans = ans + "[default = " + std::to_string(int_arguments[i]->key) + "]\n";
        }
    }
    for (int i = 0; i < flag_arguments.size(); i++) {
        ans = ans + '-' + flag_arguments[i]->short_name + ", --" + flag_arguments[i]->name +
              "=<flag>,  " + flag_arguments[i]->description_;

        if (flag_arguments[i]->default_value == true) {
            if (flag_arguments[i]->key) {
                ans = ans + "[default = true]\n";
            } else {
                ans = ans + "[default = false]\n";
            }

        }
    }

    return ans;
}

bool ArgParser::Help() {
    if (use_have_argument == false) {
        return false;
    }
    return true;
}
