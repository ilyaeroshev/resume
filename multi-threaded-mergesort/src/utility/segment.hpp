#pragma once

template <typename It>
struct Segment {
  It begin, end;

  bool IsEmpty() { return begin == end; }

  bool NonEmpty() { return begin != end; }
};