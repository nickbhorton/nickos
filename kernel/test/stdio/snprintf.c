#include <kernel/serial.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* int (*f)() must return 0 on success
 *          >0 if strings are not the same
 *          <0 if snprintf issues an error
 */
int run_test(int (*f)(), const char* test_name)
{
    int ret = f();
    if (!ret)
    {
        serial_printf("%-65.65s \x1b[32;1mpassed\x1b[0m\n", test_name);
        return 1;
    }
    else
    {
        serial_printf("%-80.80s \x1b[31;1mfailed\x1b[0m with %d\n", test_name, ret);
        return 0;
    }
};

// all functions test
int regular_string_no_f()
{
    const char* correct_result = "hello, world!";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret = snprintf(result, SERIAL_MAX_STR_LEN, "hello, world!");
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int n_less_than_str_given()
{
    const char* correct_result = "he";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret = snprintf(result, 3, "hello, world!");
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int strlen_given_is_1()
{
    const char* correct_result = "h";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret = snprintf(result, SERIAL_MAX_STR_LEN, "h");
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

// % of string length one with no specifiers need to put this on a page and
// seg boundry and make sure it does not page or seg fault.
int strlen_of_on_precent_no_spec()
{
    const char* correct_result = "";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret = snprintf(result, SERIAL_MAX_STR_LEN, "%");
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}
int format_spec_precent_precent()
{
    const char* correct_result = "%";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret = snprintf(result, SERIAL_MAX_STR_LEN, "%%");
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int s_format_spec_basic()
{
    const char* correct_result = "hello, world!";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret =
        snprintf(result, SERIAL_MAX_STR_LEN, "hello, %s!", "world");
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int s_format_spec_str_too_long()
{
    const char* correct_result = "hello, wor";
    char result[SERIAL_MAX_STR_LEN];
    // +1 again is space for the null byte that snprintf always puts in after it
    // is done writting to the buffer
    int snprintf_ret =
        snprintf(result, strlen(correct_result) + 1, "hello, %s!", "world");
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int s_format_spec_with_precision()
{
    const char* correct_result = "hello, wor!";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret =
        snprintf(result, SERIAL_MAX_STR_LEN, "hello, %.3s!", "world");
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int s_format_spec_with_precision_to_large()
{
    const char* correct_result = "hello, world!";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret =
        snprintf(result, SERIAL_MAX_STR_LEN, "hello, %.10s!", "world");
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int s_format_spec_with_precision_to_large_and_max_n()
{
    const char* correct_result = "hello, wor";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret =
        snprintf(result, strlen(correct_result) + 1, "hello, %.100s!", "world");
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int s_format_spec_with_field_width()
{
    const char* correct_result = "hello, |     world|";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret =
        snprintf(result, strlen(correct_result) + 1, "hello, |%10s|", "world");
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int s_format_spec_with_field_width_and_precision()
{
    const char* correct_result = "| nick|horto|";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret = snprintf(result, strlen(correct_result) + 1,
                                "|%5.5s|%5.5s|", "nick", "horton");
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int s_format_spec_with_large_field_width_and_small_precision()
{
    const char* correct_result = "|      nick|     horto|";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret = snprintf(result, strlen(correct_result) + 1,
                                "|%10.5s|%10.5s|", "nick", "horton");
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int s_format_spec_with_small_field_width_and_large_precision()
{
    const char* correct_result = "|horton|";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret =
        snprintf(result, strlen(correct_result) + 1, "|%5.10s|", "horton");
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int s_format_spec_field_width_and_left_align()
{
    const char* correct_result = "|nick    |";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret =
        snprintf(result, strlen(correct_result) + 1, "|%-8s|", "nick");
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int s_format_spec_field_width_to_large_and_left_align()
{
    const char* correct_result = "|nickbhorton|";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret =
        snprintf(result, strlen(correct_result) + 1, "|%-8s|", "nickbhorton");
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int s_fs_field_width_and_precision_and_left_align()
{
    const char* correct_result = "|nick      |";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret = snprintf(result, strlen(correct_result) + 1, "|%-10.4s|",
                                "nickbhorton");
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int d_format_spec_basic()
{
    const char* correct_result = "127";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret = snprintf(result, strlen(correct_result) + 1, "%d", 127);
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int d_format_spec_max()
{
    const char* correct_result = "2147483647";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret =
        snprintf(result, strlen(correct_result) + 1, "%d", 2147483647);
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int d_format_spec_min()
{
    const char* correct_result = "-2147483648";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret =
        snprintf(result, strlen(correct_result) + 1, "%d", -2147483648);
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int d_format_spec_zero()
{
    const char* correct_result = "0";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret = snprintf(result, strlen(correct_result) + 1, "%d", 0);
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int d_format_spec_neg()
{
    const char* correct_result = "-127";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret = snprintf(result, strlen(correct_result) + 1, "%d", -127);
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int d_format_spec_sign_always()
{
    const char* correct_result = "+127";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret = snprintf(result, strlen(correct_result) + 1, "%+d", 127);
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int d_format_spec_sign_align()
{
    const char* correct_result = " 127";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret = snprintf(result, strlen(correct_result) + 1, "% d", 127);
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int d_format_spec_sign_align_and_sign_always()
{
    const char* correct_result = "+127";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret =
        snprintf(result, strlen(correct_result) + 1, "% +d", 127);
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int d_format_spec_with_precision()
{
    const char* correct_result = "0127";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret =
        snprintf(result, strlen(correct_result) + 1, "%.4d", 127);
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int d_format_spec_neg_with_precision()
{
    const char* correct_result = "-0127";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret =
        snprintf(result, strlen(correct_result) + 1, "%.4d", -127);
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int d_format_spec_with_precision_and_sign_always()
{
    const char* correct_result = "+0127";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret =
        snprintf(result, strlen(correct_result) + 1, "%+.4d", 127);
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int d_format_spec_with_precision_feild_width()
{
    const char* correct_result = "    0127";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret =
        snprintf(result, strlen(correct_result) + 1, "%8.4d", 127);
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int d_format_spec_neg_with_precision_feild_width()
{
    const char* correct_result = "   -0127";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret =
        snprintf(result, strlen(correct_result) + 1, "%8.4d", -127);
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int d_format_spec_with_precision_feild_width_left_align()
{
    const char* correct_result = "0127    ";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret =
        snprintf(result, strlen(correct_result) + 1, "%-8.4d", 127);
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

int d_format_spec_with_precision_feild_width_left_align_sign_always()
{
    const char* correct_result = "+0127   ";
    char result[SERIAL_MAX_STR_LEN];
    int snprintf_ret =
        snprintf(result, strlen(correct_result) + 1, "%-+8.4d", 127);
    int strcmp_ret = strcmp(correct_result, result);
    if (snprintf_ret > 0)
    {
        return strcmp_ret;
    }
    return snprintf_ret;
}

void test_snprintf()
{
    serial_write_str("\x1b[34;1msnprintf\x1b[0m\n");
    int num_passed = 0;
    int num_attepted = 0;
    num_passed += run_test(&regular_string_no_f, "regular_string_no_f");
    num_attepted += 1;
    num_passed += run_test(&n_less_than_str_given, "n_less_than_str_given");
    num_attepted += 1;
    num_passed += run_test(&strlen_given_is_1, "strlen_given_is_1");
    num_attepted += 1;
    num_passed += run_test(&strlen_of_on_precent_no_spec, "strlen_of_on_precent_no_spec");
    num_attepted += 1;
    num_passed += run_test(&format_spec_precent_precent, "format_spec_precent_precent");
    num_attepted += 1;
    num_passed += run_test(&s_format_spec_basic, "s_format_spec_basic");
    num_attepted += 1;
    num_passed += run_test(&s_format_spec_str_too_long, "s_format_spec_str_too_long");
    num_attepted += 1;
    num_passed += run_test(&s_format_spec_with_precision, "s_format_spec_with_precision");
    num_attepted += 1;
    num_passed += run_test(&s_format_spec_with_precision_to_large,
             "s_format_spec_with_precision_to_large");
    num_attepted += 1;
    num_passed += run_test(&s_format_spec_with_precision_to_large_and_max_n,
             "s_format_spec_with_precision_to_large_and_max_n");
    num_attepted += 1;
    num_passed += run_test(&s_format_spec_with_field_width, "s_format_spec_with_field_width");
    num_attepted += 1;
    num_passed += run_test(&s_format_spec_with_field_width_and_precision,
             "s_format_spec_with_field_width_and_precision");
    num_attepted += 1;
    num_passed += run_test(&s_format_spec_with_large_field_width_and_small_precision,
             "s_format_spec_with_large_field_width_and_small_precision");
    num_attepted += 1;
    num_passed += run_test(&s_format_spec_with_small_field_width_and_large_precision,
             "s_format_spec_with_small_field_width_and_large_precision");
    num_attepted += 1;
    num_passed += run_test(&s_format_spec_field_width_and_left_align,
             "s_format_spec_field_width_and_left_align");
    num_attepted += 1;
    num_passed += run_test(&s_format_spec_field_width_to_large_and_left_align,
             "s_format_spec_field_width_to_large_and_left_align");
    num_attepted += 1;
    num_passed += run_test(&s_fs_field_width_and_precision_and_left_align,
             "s_fs_field_width_and_precision_and_left_align");
    num_attepted += 1;
    num_passed += run_test(&d_format_spec_basic, "d_format_spec_basic");
    num_attepted += 1;
    num_passed += run_test(&d_format_spec_max, "d_format_spec_max");
    num_attepted += 1;
    num_passed += run_test(&d_format_spec_min, "d_format_spec_min");
    num_attepted += 1;
    num_passed += run_test(&d_format_spec_zero, "d_format_spec_zero");
    num_attepted += 1;
    num_passed += run_test(&d_format_spec_neg, "d_format_spec_neg");
    num_attepted += 1;
    num_passed += run_test(&d_format_spec_sign_always, "d_format_spec_sign_always");
    num_attepted += 1;
    num_passed += run_test(&d_format_spec_sign_align, "d_format_spec_sign_align");
    num_attepted += 1;
    num_passed += run_test(&d_format_spec_sign_align_and_sign_always,
             "d_format_spec_sign_align_and_sign_always");
    num_attepted += 1;
    num_passed += run_test(&d_format_spec_with_precision, "d_format_spec_with_precision");
    num_attepted += 1;
    num_passed += run_test(&d_format_spec_neg_with_precision,
             "d_format_spec_neg_with_precision");
    num_attepted += 1;
    num_passed += run_test(&d_format_spec_with_precision_and_sign_always,
             "d_format_spec_with_precision_and_sign_always");
    num_attepted += 1;
    num_passed += run_test(&d_format_spec_with_precision_feild_width,
             "d_format_spec_with_precision_feild_width");
    num_attepted += 1;
    num_passed += run_test(&d_format_spec_neg_with_precision_feild_width,
             "d_format_spec_neg_with_precision_feild_width");
    num_attepted += 1;
    num_passed += run_test(&d_format_spec_with_precision_feild_width_left_align,
             "d_format_spec_with_precision_feild_width_left_align");
    num_attepted += 1;
    num_passed += run_test(&d_format_spec_with_precision_feild_width_left_align_sign_always,
             "d_format_spec_with_precision_feild_width_left_align_sign_always");
    num_attepted += 1;
    serial_printf("passed: %4d / %-4d\n", num_passed, num_attepted);
}
