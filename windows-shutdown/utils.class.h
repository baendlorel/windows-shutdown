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
    ~ClassName(){};                                   \
    ClassName(const ClassName &) = delete;            \
    ClassName &operator=(const ClassName &) = delete; \
                                                      \
    ClassName(ClassName &&) = delete;                 \
    ClassName &operator=(ClassName &&) = delete;      \
                                                      \
    static ClassName &get_instance() {                \
        static ClassName instance;                    \
        return instance;                              \
    };                                                \
                                                      \
    static ClassName *get_instance_ptr() {            \
        return &ClassName::get_instance();            \
    };                                                \
                                                      \
   private:                                           \
    ClassName() = default;

#define NO_COPY_NOR_MOVE(ClassName)                   \
   public:                                            \
    ClassName(const ClassName &) = delete;            \
    ClassName &operator=(const ClassName &) = delete; \
                                                      \
    ClassName(ClassName &&) = delete;                 \
    ClassName &operator=(ClassName &&) = delete;

#define INSTANCE_GETTER(ClassName)         \
    ~ClassName(){};                        \
    static ClassName &get_instance() {     \
        static ClassName instance;         \
        return instance;                   \
    };                                     \
                                           \
    static ClassName *get_instance_ptr() { \
        return &ClassName::get_instance(); \
    };
