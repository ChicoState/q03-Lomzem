/**
 * Unit Tests for the class
 **/

#include "Guesser.h"
#include <gtest/gtest.h>

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
