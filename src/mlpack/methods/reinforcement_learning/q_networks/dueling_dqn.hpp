/**
 * @file methods/reinforcement_learning/q_networks/dueling_dqn.hpp
 * @author Nishant Kumar
 *
 * This file contains the implementation of the dueling deep q network.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_METHODS_RL_DUELING_DQN_HPP
#define MLPACK_METHODS_RL_DUELING_DQN_HPP

#include <mlpack/prereqs.hpp>
#include <mlpack/methods/ann/ffn.hpp>
#include <mlpack/methods/ann/init_rules/gaussian_init.hpp>
#include <mlpack/methods/ann/layer/layer.hpp>
#include <mlpack/methods/ann/loss_functions/mean_squared_error.hpp>
#include <mlpack/methods/ann/loss_functions/empty_loss.hpp>

namespace mlpack {
namespace rl {

using namespace mlpack::ann;

/**
 * @tparam NetworkType The type of network used for dueling dqn.
 */
template <
  typename FeatureNetworkType = FFN<EmptyLoss<>, GaussianInitialization>,
  typename AdvantageNetworkType = Sequential<>,
  typename ValueNetworkType = Sequential<>
>
class DuelingDQN
{
 public:
  /**
   * Default constructor.
   */
  DuelingDQN() : featureNetwork(), advantageNetwork(), valueNetwork()
  { /* Nothing to do here. */ }

  /**
   * Construct an instance of DuelingDQN class.
   *
   * @param inputDim Number of inputs.
   * @param h1 Number of neurons in hiddenlayer-1.
   * @param h2 Number of neurons in hiddenlayer-2.
   * @param outputDim Number of neurons in output layer.
   */
  DuelingDQN(const int inputDim,
            const int h1,
            const int h2,
            const int outputDim):
      featureNetwork(EmptyLoss<>(), GaussianInitialization(0, 0.001)),
      valueNetwork(),
      advantageNetwork()
  {
    valueNetwork.Add(new Linear<>(h1, h2));
    valueNetwork.Add(new ReLULayer<>());
    valueNetwork.Add(new Linear<>(h2, 1));

    advantageNetwork.Add(new Linear<>(h1, h2));
    advantageNetwork.Add(new ReLULayer<>());
    advantageNetwork.Add(new Linear<>(h1, outputDim));

    Concat<> concat = new Concat<>();
    concat.Add<Sequential<>>(valueNetwork);
    concat.Add<Sequential<>>(advantageNetwork);

    featureNetwork.Add(new Linear<>(inputDim, h1));
    featureNetwork.Add(new ReLULayer<>());
    featureNetwork.Add(concat);
  }

  DuelingDQN(FeatureNetworkType featureNetwork,
             AdvantageNetworkType advantageNetwork,
             ValueNetworkType valueNetwork):
      featureNetwork(std::move(featureNetwork)),
      advantageNetwork(std::move(advantageNetwork)),
      valueNetwork(std::move(valueNetwork))
  {
    Concat<> concat = new Concat<>();
    concat.Add<Sequential<>>(valueNetwork);
    concat.Add<Sequential<>>(advantageNetwork);
    featureNetwork.Add(concat);
  }

  /**
   * Predict the responses to a given set of predictors. The responses will
   * reflect the output of the given output layer as returned by the
   * output layer function.
   *
   * If you want to pass in a parameter and discard the original parameter
   * object, be sure to use std::move to avoid unnecessary copy.
   *
   * @param state Input state.
   * @param actionValue Matrix to put output action values of states input.
   */
  void Predict(const arma::mat state, arma::mat& actionValue)
  {
    // arma::mat output, advantage, value;
    // featureNetwork.Predict(state, output);
    // actionValue = advantage.each_row() +
    //     (value - arma::mean(arma::mean(advantage)));
  }

  /**
   * Perform the forward pass of the states in real batch mode.
   *
   * @param state The input state.
   * @param output The predicted output.
   */
  void Forward(const arma::mat state, arma::mat& output)
  {
    // arma::mat output, advantage, value;
    // featureNetwork.Forward(state, output);
    // actionValue = advantage.each_row() +
    //     (value - arma::mean(arma::mean(advantage)));

    // networkOutput = output;
  }

  /**
   * Perform the backward pass of the state in real batch mode.
   *
   * @param state The input state.
   * @param target The training target.
   * @param gradient The gradient.
   */
  void Backward(const arma::mat state, arma::mat& target, arma::mat& gradient)
  {
    // arma::mat gradLoss;
    // lossFunction.Backward(networkOutput, target, gradLoss);

    // arma::mat gradValue, gradAdvantage;
    // // valueNetwork.Backward(state, gradLoss, gradValue);
    // advantageNetwork.Backward(features, gradLoss, gradAdvantage);
    // std::cout << advantageNetwork.Model()[2];
    // gradAdvantage = gradAdvantage.submat(0, 0, 1279, 0);

    // arma::mat gradSum;
    // gradSum = gradAdvantage; // + gradValue
    // featureNetwork.Backward(state, gradSum, gradient);
  }

  /**
   * Resets the parameters of the network.
   */
  void ResetParameters()
  {
    featureNetwork.ResetParameters();
    advantageNetwork.ResetParameters();
    valueNetwork.ResetParameters();
  }

  //! Return the Parameters.
  const arma::mat& Parameters() const { return featureNetwork.Parameters(); }
  //! Modify the Parameters.
  arma::mat& Parameters() { return featureNetwork.Parameters(); }

 private:
  //! Locally-stored feature network.
  FeatureNetworkType featureNetwork;

  //! Locally-stored advantage network.
  AdvantageNetworkType advantageNetwork;

  //! Locally-stored value network.
  ValueNetworkType valueNetwork;

  //! Locally-stored value network.
  arma::mat networkOutput;

  //! Locally-stored features of the network.
  arma::mat features;

  //! Locally-stored loss function.
  MeanSquaredError<> lossFunction;
};

} // namespace rl
} // namespace mlpack

#endif
