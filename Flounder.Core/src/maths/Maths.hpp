#pragma once

#include <iostream>
#include <stdexcept>
#include <cmath>
#include <string>
#include <sstream>

namespace Flounder
{
#define PI 3.14159265358979323846f
#define DEGREES_IN_CIRCLE 360.0f
#define DEGREES_IN_HALF_CIRCLE 180.0f
#define ANG2RAD PI / DEGREES_IN_HALF_CIRCLE
#define LOG_HALF log(0.5f)

	/// <summary>
	/// A class that holds many various math functions.
	/// </summary>
	class Maths
	{
	public:
		/// <summary>
		/// Converts degrees to radians.
		/// </summary>
		/// <param name="degrees"> The degrees value. </param>
		/// <returns> The radians value. </returns>
		template<typename t>
		static t Radians(t degrees)
		{
			return degrees / DEGREES_IN_HALF_CIRCLE * PI;
		}

		/// <summary>
		/// Converts radians to degrees.
		/// </summary>
		/// <param name="radians"> The radians value. </param>
		/// <returns> The degrees value. </returns>
		template<typename t>
		static t Degrees(t radians)
		{
			return radians * DEGREES_IN_HALF_CIRCLE / PI;
		}

		/// <summary>
		/// Gets the min number from the two provided.
		/// </summary>
		/// <param name="a"> The first value. </param>
		/// <param name="b"> The second value. </param>
		/// <returns> The smallest value. </returns>
		template<typename t>
		static t Min(t a, t b)
		{
			return (a < b) ? a : b;
		}

		/// <summary>
		/// Gets the max number from the two provided.
		/// </summary>
		/// <param name="a"> The first value. </param>
		/// <param name="b"> The second value. </param>
		/// <returns> The largest value. </returns>
		template<typename t>
		static t Max(t a, t b)
		{
			return (a > b) ? a : b;
		}

		/// <summary>
		/// Gets a random number.
		/// </summary>
		/// <returns> The random number. </returns>
		static float Random()
		{
			return ((float) rand()) / RAND_MAX;
		}

		/// <summary>
		/// Does a mod like the GLSL function.
		/// </summary>
		/// <param name="x"> The first value. </param>
		/// <param name="y"> The second value. </param>
		/// <returns> The resultant mod. </returns>
		template<typename t>
		static t Mod(const t &x, const t &y)
		{
			return x - y * floor(x / y);
		}

		/// <summary>
		/// Normalizes a angle into the range of 0-360.
		/// </summary>
		/// <param name="angle"> The source angle. </param>
		/// <returns> The normalized angle. </returns>
		template<typename t>
		static t NormalizeAngle(const t &angle)
		{
			if (angle >= static_cast<t>(360.0))
			{
				return angle - static_cast<t>(360.0);
			}
			if (angle < static_cast<t>(0.0))
			{
				return angle + static_cast<t>(360.0);
			}

			return angle;
		}

		/// <summary>
		/// Rounds a value to a amount of places after the decimal point.
		/// </summary>
		/// <param name="value"> The value to round. </param>
		/// <param name="place"> How many places after the decimal to round to. </param>
		/// <returns> The rounded value. </returns>
		template<typename t>
		static t RoundToPlace(const t &value, const int &place)
		{
			t placeMul = static_cast<t>(pow(10, place));
			return round(value * placeMul) / placeMul;
		}

		/// <summary>
		/// Used to floor the value if less than the min.
		/// </summary>
		/// <param name="min"> The minimum value. </param>
		/// <param name="value"> The value. </param>
		/// <returns> Returns a value with deadband applied. </returns>
		template<typename t>
		static t Deadband(const t &min, const t &value)
		{
			return fabs(value) >= fabs(min) ? value : static_cast<t>(0.0);
		}

		/// <summary>
		/// Ensures {@code value} is in the range of {@code min} to {@code max}. If {@code value} is greater than {@code max}, this will return {@code max}. If {@code value} is less than {@code min}, this will return {@code min}. Otherwise, {@code value} is returned unchanged.
		/// </summary>
		/// <param name="value"> The value to clamp. </param>
		/// <param name="min"> The smallest value of the result. </param>
		/// <param name="max"> The largest value of the result. </param>
		/// <returns> {@code value}, clamped between {@code min} and {@code max}. </returns>
		template<typename t>
		static t Clamp(const t &value, const t &min, const t &max)
		{
			return (value < min) ? min : (value > max) ? max : value;
		}

		/// <summary>
		/// Limits the value.
		/// </summary>
		/// <param name="value"> The value. </param>
		/// <param name="limit"> The limit. </param>
		/// <returns> A limited value. </returns>
		template<typename t>
		static t Limit(const t &value, const t &limit)
		{
			return value > limit ? limit : value;
		}

		/// <summary>
		/// Checks if two values are almost equal.
		/// </summary>
		/// <param name="a"> The first value. </param>
		/// <param name="b"> The second value. </param>
		/// <param name="eps"> EPS is the measure of equality. </param>
		/// <returns> If both are almost equal. </returns>
		template<typename t>
		static bool AlmostEqual(const t &a, const t &b, const t &eps)
		{
			return fabs(a - b) < eps;
		}

		/// <summary>
		/// Interpolates two values by a blendFactor using cos interpolation.
		/// </summary>
		/// <param name="a"> The first value. </param>
		/// <param name="b"> The second value. </param>
		/// <param name="blend"> The blend value. </param>
		/// <returns> Returns a interpolated value. </returns>
		template<typename t>
		static t CosInterpolate(const t &a, const t &b, const t &blend)
		{
			float ft = static_cast<t>(blend) * PI;
			float f = 1.0f - cos(ft) * 0.5f;
			return a * static_cast<t>(1.0f - f) + b * f;
		}

		/// <summary>
		/// A calculation that steps smoothly between two edges.
		/// </summary>
		/// <param name="edge0"> The inner edge. </param>
		/// <param name="edge1"> The outer edge. </param>
		/// <param name="x"> The sample. </param>
		/// <returns> The resulting stepped value. </returns>
		template<typename t>
		static t SmoothlyStep(const t &edge0, const t &edge1, const t &x)
		{
			t s = Clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
			return s * s * (static_cast<t>(3.0 - 2.0) * s);
		}

		/// <summary>
		/// Generates a random value from between a range.
		/// </summary>
		/// <param name="min"> The min value. </param>
		/// <param name="max"> The max value. </param>
		/// <returns> The randomly selected value within the range. </returns>
		template<typename t>
		static t RandomInRange(const t &min, const t &max)
		{
			t range = max - min;
			float scaled = Random();
			scaled *= static_cast<float>(range);
			return static_cast<t>(scaled) + min; // == (rand.nextDouble() * (max-min)) + min;
		}

		/// <summary>
		/// Creates a number between two numbers, logarithmic.
		/// </summary>
		/// <param name="lowerLimit"> The lower number. </param>
		/// <param name="upperLimit"> The upper number. </param>
		/// <returns> The final random number. </returns>
		static double LogRandom(const double &lowerLimit, const double &upperLimit)
		{
			double logLower = log(lowerLimit);
			double logUpper = log(upperLimit);

			double raw = RandomInRange(0.0f, 1.0f);
			double result = exp(raw * (logUpper - logLower) + logLower);

			if (result < lowerLimit)
			{
				result = lowerLimit;
			}
			else if (result > upperLimit)
			{
				result = upperLimit;
			}

			return result;
		}

		/// <summary>
		/// Generates a single value from a normal distribution, using Box-Muller.
		/// https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
		/// </summary>
		/// <param name="standardDeviation"> The standards deviation of the distribution. </param>
		/// <param name="mean"> The mean of the distribution. </param>
		/// <returns> A normally distributed value. </returns>
		static float NormallyDistributedSingle(const float &standardDeviation, const float &mean)
		{
			// Intentionally duplicated to avoid IEnumerable overhead.
			double u1 = RandomInRange(0.0f, 1.0f);
			double u2 = RandomInRange(0.0f, 1.0f);

			double x1 = sqrt(-2.0 * log(u1));
			double x2 = 2.0 * PI * u2;
			double z1 = x1 * sin(x2); // Random normal(0,1)
			return static_cast<float>(z1) * standardDeviation + mean;
		}
	};
}