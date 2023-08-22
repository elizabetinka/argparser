#pragma once
#include <iostream>
#include <vector>

namespace ArgumentParser {
    class ArgParser{
    private:
        class StringArgument{
            friend ArgParser;
        public:
            StringArgument(const std::string& param)
            :name{param}{}
            StringArgument(char p,const std::string& param)
                :name{param},short_name{p}{}

            void Default(const std::string& value){
                key=value;
                default_value= true;
                return;
            }
            void StoreValue(std::string& value){
                value=key;
                partner_key.push_back(&value);
                return;
            }
            StringArgument& MultiValue(size_t min_count){
                multi_value=true;
                min_args_count=min_count;
                return *this;
            }
        private:
            char short_name=' ';
            bool default_value=false;
            bool multi_value=false;
            size_t min_args_count=0;
            std::vector<std::string> keys;
            std::vector<std::string*> partner_key;
            std::string name;
            std::string key;
            std::string description_;
        };
        class IntArgument{
            friend ArgParser;
        public:
            IntArgument(const std::string& param)
            :name{param}{}
            IntArgument(char p,const std::string& param)
            :name{param},short_name{p}{}
            void Default(int value){
                key=value;
                default_value= true;
                return;
            }
            void StoreValue(int& value){
                value=key;
                partner_key.push_back(&value);
                return;
            }
            void StoreValues(std::vector<int>& int_values){
                int_values=keys;
                partner_key_multi.push_back(&int_values);
                return;
            }
            IntArgument& MultiValue(){
                multi_value=true;
                return *this;
            }
            IntArgument& MultiValue(size_t min_count){
                multi_value=true;
                min_args_count=min_count;
                return *this;
            }
            IntArgument& Positional(){
                positional=true;
                return *this;
            }


        private:
            char short_name=' ';
            bool default_value=false;
            bool positional=false;
            bool multi_value=false;
            size_t min_args_count=0;
            std::vector<int*> partner_key;
            std::vector<std::vector<int>*> partner_key_multi;
            std::vector<int> keys;
            std::string description_;
            std::string name;
            int key;
        };
        class FlagArgument{
            friend ArgParser;
        public:
            FlagArgument(const std::string& param)
            :name{param}{}
            FlagArgument(char p,const std::string& param)
            :name{param},short_name{p}{}
            void Default(bool value){
                key=value;
                default_value= true;
                return;
            }
            void StoreValue(bool& value){
                value=key;
                partner_key.push_back(&value);
                return;
            }

        private:
            char short_name=' ';
            bool default_value=false;
            std::string name;
            std::string description_;
            bool key=false;
            std::vector<bool*> partner_key;
        };

        std::vector<StringArgument*> string_arguments;
        std::vector<IntArgument*> int_arguments;
        std::vector<FlagArgument*> flag_arguments;

        std::string long_name_help_argument;
        char short_name_help_argument;
        std::string description_help_argument;
        bool having_help_argument=false;
        bool use_have_argument=false;

    public:
        std::string parser_name;
        ArgParser(const std::string& s);

        bool Parse(int argc, char** argv);
        bool Parse(const std::vector<std::string>& mass);

        StringArgument& AddStringArgument(const std::string& longer,const std::string& description="");
        StringArgument& AddStringArgument(char shorter,const std::string& longer,const std::string& description="");

        const std::string& GetStringValue(const std::string& s,size_t index=0);

        IntArgument& AddIntArgument(const std::string& longer,const std::string& description="");
        IntArgument& AddIntArgument(char shorter,const std::string& longer,const std::string& description="");

        int GetIntValue(const std::string& s, size_t index=0);

        FlagArgument& AddFlag(const std::string& longer,const std::string& description="");
        FlagArgument& AddFlag(char shorter,const std::string& longer,const std::string& description="");

        bool GetFlag(const std::string& s);

        void AddHelp(char shorter,const std::string& longer,const std::string& description);
        const std::string HelpDescription();
        bool Help();

        ~ArgParser(){
            for (int i=0;i<string_arguments.size();i++){
                delete string_arguments[i];
            }
            for (int i=0;i<int_arguments.size();i++){
                delete int_arguments[i];
            }
            for (int i=0;i<flag_arguments.size();i++){
                delete flag_arguments[i];
            }
        };
    };

} // namespace ArgumentParser