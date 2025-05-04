/**
 * Unit Tests for the class
 **/

#include "Guesser.h"
#include <gtest/gtest.h>

/* How many guesses the Guesser object begins with */
const unsigned int STARTING_GUESSES = 3;

/* The max number of characters allowed in a secret */
const unsigned int MAX_SECRET_LENGTH = 32;

class GuesserTest : public ::testing::Test {
  protected:
    GuesserTest() {
    } // constructor runs before each test
    virtual ~GuesserTest() {
    } // destructor cleans up after tests
    virtual void SetUp() {
    } // sets up before each test (after constructor)
    virtual void TearDown() {
    } // clean up after each test, (before destructor)
};

/* Tests constructor behavior */

TEST(GuesserTest, constructor_truncate_doesnt_match_original) {
    /* Tests this behavior:
     * This secret will not change for the lifespan of an instance
     of any Guesser object and must have a length of 32 characters or less,
     otherwise, it will be truncated at that length.
     */

    /* Assumes MAX_SECRET_LENGTH == 32 */
    string longInput =
        "Thi$ is a long input that goes well beyond the max limit of the "
        "secret. This is to test whether the constructor correctly truncates "
        "strings according to the expected behavior.";
    Guesser object(longInput);

    ASSERT_FALSE(object.match(longInput));
}

TEST(GuesserTest, constructor_truncate_matches) {
    /* Tests this behavior:
     * This secret will not change for the lifespan of an instance
     of any Guesser object and must have a length of 32 characters or less,
     otherwise, it will be truncated at that length.
     */

    /* Assumes MAX_SECRET_LENGTH == 32 */
    string longInput =
        "Thi$ is a long input that goes well beyond the max limit of the "
        "secret. This is to test whether the constructor correctly truncates "
        "strings according to the expected behavior.";
    Guesser object(longInput);

    string truncatedInput = longInput.substr(0, MAX_SECRET_LENGTH);

    ASSERT_TRUE(object.match(truncatedInput));
}

/* Tests for match behavior while unlocked */

TEST(GuesserTest, match_both_empty) {
    Guesser object("");
    ASSERT_TRUE(object.match(""));
}

TEST(GuesserTest, match_both_one_letter_same) {
    Guesser object("a");
    ASSERT_TRUE(object.match("a"));
}

TEST(GuesserTest, match_both_one_letter_diff) {
    Guesser object("a");
    ASSERT_FALSE(object.match("b"));
}

TEST(GuesserTest, match_upper_lower_diff_one_char) {
    Guesser object("a");
    ASSERT_FALSE(object.match("A"));
}

TEST(GuesserTest, match_upper_lower_diff_word) {
    Guesser object("seCret");
    ASSERT_FALSE(object.match("secret"));
}

TEST(GuesserTest, match_upper_lower_same) {
    Guesser object("seCret");
    ASSERT_TRUE(object.match("seCret"));
}

TEST(GuesserTest, match_special_char_diff) {
    Guesser object("^this is my-$ecret!");
    ASSERT_FALSE(object.match("^this is my-secret!"));
}

TEST(GuesserTest, match_special_char_same) {
    Guesser object("^this is my-$ecret!");
    ASSERT_TRUE(object.match("^this is my-$ecret!"));
}

TEST(GuesserTest, remaining_no_guess) {
    Guesser object("Secret");
    ASSERT_EQ(object.remaining(), STARTING_GUESSES);
}

TEST(GuesserTest, remaining_correct_guesses_dont_decrement) {
    Guesser object("Secret");
    object.match("Secret");
    object.match("Secret");
    object.match("Secret");
    object.match("Secret");
    object.match("Secret");
    object.match("Secret");
    ASSERT_EQ(STARTING_GUESSES, object.remaining());
}

TEST(GuesserTest, match_multiple_correct) {
    Guesser object("Secret");
    object.match("Secret");
    object.match("Secret");
    object.match("Secret");
    object.match("Secret");
    object.match("Secret");
    object.match("Secret");
    object.match("Secret");
    object.match("Secret");
    object.match("Secret");
    ASSERT_TRUE(object.match("Secret"));
}

/* Test lock behavior */

TEST(GuesserTest, remaining_two_guess_close) {
    Guesser object("Secret");
    object.match("secret");
    object.match("secret");
    ASSERT_EQ(object.remaining(), STARTING_GUESSES - 2);
}

TEST(GuesserTest, match_one_close_attempt) {
    Guesser object("Secret");
    object.match("secret");
    ASSERT_TRUE(object.match("Secret"));
}

TEST(GuesserTest, match_out_of_attempts) {
    Guesser object("Secret");
    /* Assumes STARTING_GUESSES == 3 */
    object.match("secret");
    object.match("SecreT");
    object.match("sEcret");
    ASSERT_FALSE(object.match("Secret"));
}

TEST(GuesserTest, match_attempts_past_remaining) {
    Guesser object("Secret");
    /* Assumes STARTING_GUESSES <= 3 */
    object.match("secret");
    object.match("SecreT");
    object.match("sEcret");
    object.match("sEcreT");
    object.match("sEcrEt");
    object.match("sEcRet");
    object.match("sEcRet");
    object.match("sEcrET");
    object.match("sEcreT");
    object.match("sEcreT");
    object.match("sEcREt");
    ASSERT_FALSE(object.match("Secret"));
}

TEST(GuesserTest, match_one_last_attempt) {
    Guesser object("Secret");
    /* Assumes STARTING_GUESSES == 3 */
    object.match("secret");
    object.match("SecreT");
    ASSERT_TRUE(object.match("Secret"));
}

TEST(GuesserTest, match_far_guess_locks) {
    Guesser object("Secret");
    object.match("s");
    ASSERT_FALSE(object.match("Secret"));
}

TEST(GuesserTest, match_correct_resets_to_max) {
    /* Tests this behavior:
     * Whenever an unlocked secret is guessed with a true match, the guesses
     remaining reset to three (3)
     */

    /* Assumes STARTING_GUESSES == 3 */

    Guesser object("Secret");
    object.match("secret");
    object.match("secret");

    /* Assumes unlocked at this point */
    EXPECT_TRUE(object.match("Secret"));

    /* Should have reset to 3 */
    object.match("secret");
    object.match("secret");
    ASSERT_TRUE(object.match("Secret"));
}

TEST(GuesserTest, match_far_lock_hides_remaining) {
    /* Tests this behavior:
     * If the secret is locked for any other reason, such
     as a big distance in a guess, the count of remaining guesses should still
     count down as usual to hide that the secret has been locked.
     */

    Guesser object("Secret");
    EXPECT_FALSE(object.match(
        "super-omega way far off guess that are not even same length"));

    /* Assumes STARTING_GUESSES == 3 */
    /* After one guess, remaining() should be STARTING_GUESSES - 1 */
    ASSERT_EQ(STARTING_GUESSES - 1, object.remaining());
}

TEST(GuesserTest, match_far_lock_decrements_remaining) {
    /* Tests this behavior:
     * If the secret is locked for any other reason, such
     as a big distance in a guess, the count of remaining guesses should still
     count down as usual to hide that the secret has been locked.
     */

    Guesser object("Secret");
    EXPECT_FALSE(object.match(
        "super-omega way far off guess that are not even same length"));
    EXPECT_FALSE(object.match(
        "super-omega way far off guess that are not even same length"));
    EXPECT_FALSE(object.match(
        "super-omega way far off guess that are not even same length"));
    ASSERT_EQ(STARTING_GUESSES - 3, object.remaining());
}
