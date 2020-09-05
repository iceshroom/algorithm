#include <string>

#ifndef ERR_H_
#define ERR_H_

template <typename T>
class err
{
        private:
                std::string errstr;

        public:
                void operator()(char * str)
                {
                        errstr = str;
                        printf("%s Error string : %s\n", errstr , strerror(errno));
                        exit(1);
                }

                void operator()(T * err_object_here, char * str)
                {
                        errstr = str;
                        printf("%s Error string : %s\n", errstr , strerror(errno));
                        delete err_object_here;
                        exit(1);
                }
};

#endif