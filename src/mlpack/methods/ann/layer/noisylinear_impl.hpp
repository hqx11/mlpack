/**
 * @file methods/ann/layer/noisylinear_impl.hpp
 * @author Nishant Kumar
 *
 * Implementation of the NoisyLinear layer class.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_METHODS_ANN_LAYER_NOISYLINEAR_IMPL_HPP
#define MLPACK_METHODS_ANN_LAYER_NOISYLINEAR_IMPL_HPP

// In case it hasn't yet been included.
#include "noisylinear.hpp"

namespace mlpack {
namespace ann /** Artificial Neural Network. */ {

template<typename InputDataType, typename OutputDataType>
NoisyLinear<InputDataType, OutputDataType>::NoisyLinear() :
    inSize(0),
    outSize(0)
{
  // Nothing to do here.
}

template<typename InputDataType, typename OutputDataType>
NoisyLinear<InputDataType, OutputDataType>::NoisyLinear(
    const size_t inSize,
    const size_t outSize) :
    inSize(inSize),
    outSize(outSize)
{
  weights.set_size(outSize * inSize + outSize, 1);
}

template<typename InputDataType, typename OutputDataType>
void NoisyLinear<InputDataType, OutputDataType>::Reset()
{
  weight = arma::mat(weights.memptr(), outSize, inSize, false, false);
  bias = arma::mat(weights.memptr() + weight.n_elem,
      outSize, 1, false, false);
}

template<typename InputDataType, typename OutputDataType>
template<typename eT>
void NoisyLinear<InputDataType, OutputDataType>::Forward(
    const arma::Mat<eT>& input, arma::Mat<eT>& output)
{
  output = weight * input;
  output.each_col() += bias;
}

template<typename InputDataType, typename OutputDataType>
template<typename eT>
void NoisyLinear<InputDataType, OutputDataType>::Backward(
    const arma::Mat<eT>& /* input */, const arma::Mat<eT>& gy, arma::Mat<eT>& g)
{
  g = weight.t() * gy;
}

template<typename InputDataType, typename OutputDataType>
template<typename eT>
void NoisyLinear<InputDataType, OutputDataType>::Gradient(
    const arma::Mat<eT>& input,
    const arma::Mat<eT>& error,
    arma::Mat<eT>& gradient)
{
  gradient.submat(0, 0, weight.n_elem - 1, 0) = arma::vectorise(
      error * input.t());
  gradient.submat(weight.n_elem, 0, gradient.n_elem - 1, 0) =
      arma::sum(error, 1);
}

template<typename InputDataType, typename OutputDataType>
template<typename Archive>
void NoisyLinear<InputDataType, OutputDataType>::serialize(
    Archive& ar, const unsigned int /* version */)
{
  ar & BOOST_SERIALIZATION_NVP(inSize);
  ar & BOOST_SERIALIZATION_NVP(outSize);

  // This is inefficient, but we have to allocate this memory so that
  // WeightSetVisitor gets the right size.
  if (Archive::is_loading::value)
    weights.set_size(outSize * inSize + outSize, 1);
}

} // namespace ann
} // namespace mlpack

#endif
