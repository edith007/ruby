require_relative '../../spec_helper'
require 'bigdecimal'

describe "BigDecimal#remainder" do

  before :each do
    @zero = BigDecimal("0")
    @one = BigDecimal("1")
    @three = BigDecimal("3")
    @mixed = BigDecimal("1.23456789")
    @pos_int = BigDecimal("2E5555")
    @neg_int = BigDecimal("-2E5555")
    @pos_frac = BigDecimal("2E-9999")
    @neg_frac = BigDecimal("-2E-9999")
    @nan = BigDecimal("NaN")
    @infinity = BigDecimal("Infinity")
    @infinity_minus = BigDecimal("-Infinity")
    @one_minus = BigDecimal("-1")
    @frac_1 = BigDecimal("1E-99999")
    @frac_2 = BigDecimal("0.9E-99999")
  end

  it "it equals modulo, if both values are of same sign" do
    BigDecimal('1234567890123456789012345679').remainder(BigDecimal('1')).should == @zero
    BigDecimal('123456789').remainder(BigDecimal('333333333333333333333333333E-50')).should == BigDecimal('0.12233333333333333333345679E-24')

    @mixed.remainder(@pos_frac).should == @mixed % @pos_frac
    @pos_int.remainder(@pos_frac).should == @pos_int % @pos_frac
    @neg_frac.remainder(@neg_int).should == @neg_frac % @neg_int
    @neg_int.remainder(@neg_frac).should == @neg_int % @neg_frac
  end

  it "means self-arg*(self/arg).truncate" do
    @mixed.remainder(@neg_frac).should == @mixed - @neg_frac * (@mixed / @neg_frac).truncate
    @pos_int.remainder(@neg_frac).should == @pos_int - @neg_frac * (@pos_int / @neg_frac).truncate
    @neg_frac.remainder(@pos_int).should == @neg_frac - @pos_int * (@neg_frac / @pos_int).truncate
    @neg_int.remainder(@pos_frac).should == @neg_int - @pos_frac * (@neg_int / @pos_frac).truncate
  end

  it "returns NaN used with zero" do
    @mixed.remainder(@zero).should.nan?
    @zero.remainder(@zero).should.nan?
  end

  it "returns zero if used on zero" do
    @zero.remainder(@mixed).should == @zero
  end

  it "returns NaN if NaN is involved" do
    @nan.remainder(@nan).should.nan?
    @nan.remainder(@one).should.nan?
    @one.remainder(@nan).should.nan?
    @infinity.remainder(@nan).should.nan?
    @nan.remainder(@infinity).should.nan?
  end

  ruby_version_is ""..."3.3" do
    it "returns NaN if Infinity is involved" do
      @infinity.remainder(@infinity).should.nan?
      @infinity.remainder(@one).should.nan?
      @infinity.remainder(@mixed).should.nan?
      @infinity.remainder(@one_minus).should.nan?
      @infinity.remainder(@frac_1).should.nan?
      @one.remainder(@infinity).should.nan?

      @infinity_minus.remainder(@infinity_minus).should.nan?
      @infinity_minus.remainder(@one).should.nan?
      @one.remainder(@infinity_minus).should.nan?
      @frac_2.remainder(@infinity_minus).should.nan?

      @infinity.remainder(@infinity_minus).should.nan?
      @infinity_minus.remainder(@infinity).should.nan?
    end
  end

  it "coerces arguments to BigDecimal if possible" do
    @three.remainder(2).should == @one
  end

  describe "with Object" do
    it "tries to coerce the other operand to self" do
      object = mock("Object")
      object.should_receive(:coerce).with(@three).and_return([@three, 2])
      @three.remainder(object).should == @one
    end
  end

  it "raises TypeError if the argument cannot be coerced to BigDecimal" do
    -> {
      @one.remainder('2')
    }.should raise_error(TypeError)
  end

end
