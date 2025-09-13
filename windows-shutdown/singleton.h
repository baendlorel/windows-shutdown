#pragma once

#define SINGLETON(ClassName)                         \
   public:                                           \
    ClassName(const ClassName&) = delete;            \
    ClassName& operator=(const ClassName&) = delete; \
                                                     \
    ClassName(ClassName&&) = delete;                 \
    ClassName& operator=(ClassName&&) = delete;      \
                                                     \
    static ClassName& GetInstance() {                \
        static ClassName instance;                   \
        return instance;                             \
    };                                               \
                                                     \
   private:                                          \
    ClassName() = default;\
