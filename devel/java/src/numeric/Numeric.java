package com.googlecode.promotelib.numeric;

public final class Numeric
{
  public static final int powerOf2Above(final int x, final boolean inclusive)
  {
    int result = inclusive ? x - 1 : x;
    result |= result >> 1;
    result |= result >> 2;
    result |= result >> 4;
    result |= result >> 8;
    result |= result >> 16;
    return result + 1;
  }

  public static final int powerOf2Below(final int x, final boolean inclusive)
  {
    return powerOf2Above(x, !inclusive) >> 1;
  }

  public static final long powerOf2Above(final long x, final boolean inclusive)
  {
    long result = inclusive ? x - 1 : x;
    result |= result >> 1;
    result |= result >> 2;
    result |= result >> 4;
    result |= result >> 8;
    result |= result >> 16;
    result |= result >> 32;
    return result + 1;
  }

  public static final long powerOf2Below(final long x, final boolean inclusive)
  {
    return powerOf2Above(x, !inclusive) >> 1;
  }

  private Numeric() { }
}

