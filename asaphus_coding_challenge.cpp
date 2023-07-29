[20:17] Maha  BOUJMIL

 #include <algorithm>

#include <cstdint>

#include <iostream>

#include <limits>

#include <list>

#include <memory>

#include <numeric>

#include <vector>


#define CATCH_CONFIG_MAIN

#include "catch.hpp"


class Box {

public:

    explicit Box(double initial_weight) : weight_(initial_weight) {}

    static std::unique_ptr<Box> makeGreenBox(double initial_weight);

    static std::unique_ptr<Box> makeBlueBox(double initial_weight);

    bool operator<(const Box& rhs) const { return weight_ < rhs.weight_; }


    virtual void absorbToken(double weight) = 0;

    virtual double calculateScore() = 0;


protected:

    double weight_;

};


// ***************************** GreenBox *****************************


class GreenBox : public Box {

public:

    explicit GreenBox(double initial_weight) : Box(initial_weight) {}

    void absorbToken(double weight);

    double calculateScore();


private:

    std::deque<double> recentWeightsDeque;

};


void GreenBox::absorbToken(double weight) {

    std::cout << "GreenBox chosen with weight : " << weight_ << " token weight : " << weight << std::endl;

    recentWeightsDeque.push_back(weight);

    if (recentWeightsDeque.size() > 3) {

        recentWeightsDeque.pop_front();

    }


    weight_ += weight;

}


double GreenBox::calculateScore() {

    double mean = 0.0;

    int num_weights = recentWeightsDeque.size();

    int count = std::min(num_weights, 3);


    for (auto wght : recentWeightsDeque) {

        mean += wght;

    }

    mean /= count;

    std::cout << "GreenBox::calculateScore = " << mean * mean << std::endl;

    return mean * mean;

}


// ***************************** BlueBox *****************************


class BlueBox : public Box {

public:

    explicit BlueBox(double initial_weight) : Box(initial_weight) {}

    void absorbToken(double weight);

    double calculateScore();


private:

    double minWeight = 0;

    double maxWeight = 0;


};


void BlueBox::absorbToken(double weight) {

    std::cout << "BlueBox chosen with weight " << weight_ << " token weight : " << weight << std::endl;


    if ((maxWeight == 0) && (minWeight == 0)) {

        maxWeight = weight;

        minWeight = weight;

    }

    else {

        maxWeight = std::max(maxWeight, weight);

        minWeight = std::min(minWeight, weight);

    }


    weight_ += weight;

}


double BlueBox::calculateScore() {

    std::cout << "BlueBox::calculateScore " << (double)((0.5) * (minWeight + maxWeight) * (minWeight + maxWeight + 1) + maxWeight) << std::endl;

    return (double)((0.5) * (minWeight + maxWeight) * (minWeight + maxWeight + 1) + maxWeight);

}


// ***************************** Player *****************************


class Player {

public:

    void takeTurn(uint32_t input_weight, const std::vector<std::unique_ptr<Box> >& boxes) {

        auto it = std::min_element(boxes.begin(), boxes.end(), [](const std::unique_ptr<Box> &box1P, const std::unique_ptr<Box> &box2P) {

                                                                        return *(box1P) < *(box2P);

                                                                        });

        (*it)->absorbToken(input_weight);

        score_ += (*it)->calculateScore();

    }

    double getScore() const { return score_; }


private:

    double score_{0.0};

};


// The 'play' function remains unchanged.

// Implement the test cases for "GreenBox" and "BlueBox" absorption functions below.


TEST_CASE("Test absorption of green box", "[green]") {

    std::unique_ptr<Box> greenBox = Box::makeGreenBox(0.0);

    REQUIRE(greenBox->calculateScore() == 0.0);


    greenBox->absorbToken(1.0);

    REQUIRE(greenBox->calculateScore() == 1.0);


    greenBox->absorbToken(2.0);

    REQUIRE(greenBox->calculateScore() == 2.25);


    greenBox->absorbToken(3.0);

    REQUIRE(greenBox->calculateScore() == 4.0);

}


TEST_CASE("Test absorption of blue box", "[blue]") {

    std::unique_ptr<Box> blueBox = Box::makeBlueBox(0.0);

    REQUIRE(blueBox->calculateScore() == 0.0);


    blueBox->absorbToken(5.0);

    REQUIRE(blueBox->calculateScore() == 15.0);


    blueBox->absorbToken(3.0);

    REQUIRE(blueBox->calculateScore() == 15.0);


    blueBox->absorbToken(8.0);

    REQUIRE(blueBox->calculateScore() == 52.0);

}
