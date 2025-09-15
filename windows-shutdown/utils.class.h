// ReSharper disable CppClangTidyClangDiagnosticUnusedMacros
// ReSharper disable CppClangTidyBugproneMacroParentheses
#pragma once

#define NO_COPY_DEFAULT_MOVE(ClassName)               \
   public:                                            \
    ClassName(const ClassName &) = delete;            \
    ClassName &operator=(const ClassName &) = delete; \
    ClassName(ClassName &&) noexcept = default;       \
    ClassName &operator=(ClassName &&) noexcept = default;

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
