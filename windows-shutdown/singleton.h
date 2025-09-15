#pragma once

#define SINGLETON(ClassName)                          \
   public:                                            \
    ~ClassName();                                     \
    ClassName(const ClassName &) = delete;            \
    ClassName &operator=(const ClassName &) = delete; \
                                                      \
    ClassName(ClassName &&) = delete;                 \
    ClassName &operator=(ClassName &&) = delete;      \
                                                      \
    static ClassName &GetInstance() {                 \
        static ClassName instance;                    \
        return instance;                              \
    };                                                \
                                                      \
    static ClassName *GetInstancePtr() {              \
        return &ClassName::GetInstance();             \
    };                                                \
                                                      \
   private:                                           \
    ClassName() = default;

#define DELETE_COPY_AND_MOVE(ClassName)               \
   public:                                            \
    ~ClassName();                                     \
    ClassName(const ClassName &) = delete;            \
    ClassName &operator=(const ClassName &) = delete; \
                                                      \
    ClassName(ClassName &&) = delete;                 \
    ClassName &operator=(ClassName &&) = delete;      \
                                                      \
    static ClassName &GetInstance() {                 \
        static ClassName instance;                    \
        return instance;                              \
    };                                                \
                                                      \
    static ClassName *GetInstancePtr() {              \
        return &ClassName::GetInstance();             \
    };
