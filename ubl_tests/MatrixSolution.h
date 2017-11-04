#pragma once

#include <vector>
#include <map>


using namespace std;

class Solution {
public:
	using PointType = std::pair<int, int>;


	vector<vector<int>> updateMatrix(vector<vector<int>>& matrix) {
		// get all zeros possible:
		for (int i = 0; i<matrix.size(); ++i) {
			for (int j = 0; j<matrix[i].size(); ++j) {
				if (matrix[i][j] == 1) {
					const PointType thisPoint = std::make_pair<>(i, j);
					int step = 1;
					while (step < matrix.size()) {
						bool isFound = false;
						int min_dist = std::numeric_limits<int>::max();

						for (int k = 0; k<4; ++k) {
							for (int s = -step; s <= step; ++s) {
								bool h_traversal = k % 2 == 0;
								int opposite_step = k >= 2 ? step : -step;

								int ii = h_traversal ? s : opposite_step;
								int jj = h_traversal ? opposite_step : s;

								const PointType point = std::make_pair<>(i + ii, j + jj);
								if (is_point_in_bounds(point, matrix.size())) {
									if (matrix[point.first][point.second] == 0) {
										int curr_dist = get_distance(thisPoint, point);
										min_dist = std::min(curr_dist, min_dist);
										isFound = true;
									}
								}
							}
						}

						if (isFound) {
							matrix[i][j] = min_dist;
							break;
						}

						++step;
					}
				}
			}
		}

		return matrix;
	}


	inline bool is_point_in_bounds(const PointType& point, int size)
	{
		return (point.first >= 0) && (point.first < size) &&
			(point.second >= 0) && (point.second < size);
	}


public:
	vector<vector<int>> bruteForceSolution(vector<vector<int>>& matrix) {
		std::vector<PointType> zeroPoints;

		// get all zeros possible:
		for (int i = 0; i<matrix.size(); ++i) {
			for (int j = 0; j<matrix[i].size(); ++j) {
				if (matrix[i][j] == 0) {
					zeroPoints.push_back(std::make_pair<>(i, j));
				}
			}
		}

		// Fill necessery data:
		for (int i = 0; i<matrix.size(); ++i) {
			for (int j = 0; j<matrix[i].size(); ++j) {
				if (matrix[i][j] == 1) {
					PointType onePoint = std::make_pair<>(i, j);
					int min_distance = std::numeric_limits<int>::max();
					for (const auto& point : zeroPoints) {
						auto dist = get_distance(onePoint, point);
						min_distance = std::min(dist, min_distance);

						if (min_distance == 1) {
							break;
						}
					}
					matrix[i][j] = min_distance;
				}
			}
		}

		return matrix;
	}


	inline int get_distance(const PointType& p1, const PointType& p2)
	{
		return std::abs(p1.first - p2.first) + std::abs(p1.second - p2.second);
	}
};


