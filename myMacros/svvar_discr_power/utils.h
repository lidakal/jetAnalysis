template<typename T>
std::vector<T> flatten(std::vector<std::vector<T>> const &vec)
{
  std::vector<T> flattened;
  for (auto const &v: vec) {
    flattened.insert(flattened.end(), v.begin(), v.end());
  }
  return flattened;
}