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

}
