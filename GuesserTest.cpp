/**
 * Unit Tests for the class
 **/

#include "Guesser.h"
#include <gtest/gtest.h>

/* How many guesses the Guesser object begins with */
const unsigned int STARTING_GUESSES = 3;

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

TEST(GuesserTest, match_upper_lower_diff) {
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

    /* Should have reset */
    object.match("secret");
    object.match("secret");
    ASSERT_TRUE(object.match("Secret"));
}
