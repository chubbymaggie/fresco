/*$Id: UnicodePluginGenerator.pl,v 1.7 2003/08/01 16:47:16 tobias Exp 16A0-16FF.cc
 *
 * This source file is a part of the Berlin Project
 * Copyright (C) 1999-2003 Tobias Hunger <tobias@fresco.org>
 * http://www.fresco.org
 *
 * It was automatically created from the files available at
 * ftp.unicode.org on Tue, 05 Aug 2003 17:09:43 +0200.
 *
 * This plugin to libPrague is free software; you can redistribute it
 * and/or  modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 675 Mass Ave, Cambridge,
 * MA 02139, USA
 */

#include <Babylon/defs.hh>
#include <Babylon/internal/Blocks.hh>
#include <bitset>
#include <utility>


namespace Babylon
{
  namespace Module
  {
    class Runic16A0 : public Babylon::Block
    {
      public:
        void clean() { };

        Runic16A0() :
        my_first_letter(0x16A0),
        my_last_letter(0x16FF)
        // my_version="4.0" // Not yet supported!
        {
        }

        ~Runic16A0() { }

        UCS4 first_letter() const
        {
        return my_first_letter;
    }  

    UCS4 last_letter() const
    {
        return my_last_letter;
    }

    bool is_undef_block() const
    {
        return 0;
    }

    // query functions:
    std::string blockname(const UCS4 uc) const
    {
        return "Runic";
    }

        bool is_defined(const UCS4 uc) const
        {
            return (my_is_defined.test(uc - my_first_letter));
        }

        UCS4 uppercase(const UCS4 uc) const
        {
            return uc;
        }

        UCS4 lowercase(const UCS4 uc) const
        {
            return uc;
        }

        UCS4 titlecase(const UCS4 uc) const
        {
            return uc;
        }

        int dec_digit_value(const UCS4 uc) const
        {
            return 0;
        }

        bool is_Decimal_Digit(const UCS4 uc) const
        {
            return 0;
        }

        int digit_value(const UCS4 uc) const
        {
            return 0;
        }

        bool is_Digit(const UCS4 uc) const
        {
            return 0;
        }

        float numeric_value(const UCS4 uc) const
        {
            if (!is_defined(uc))
                return 0;
            switch(uc)
            {
            case 0x16EEu:
                return 17.000000;
            case 0x16EFu:
                return 18.000000;
            case 0x16F0u:
                return 19.000000;
            default:
                return 0;
            }
        }

        bool is_Numeric(const UCS4 uc) const
        {
            switch(uc)
            {
            case 0x16EEu:
            case 0x16EFu:
            case 0x16F0u:
                return 1;
            default:
                return 0;
            }
        }

        Gen_Cat category(const UCS4 uc) const
        {
            if (!is_defined(uc))
                return CAT_MAX;
            return Babylon::Gen_Cat(Runic16A0::my_cat[uc - my_first_letter]);
        }

        Can_Comb_Class comb_class(const UCS4 uc) const
        {
            if (!is_defined(uc))
                return CC_MAX;
            return Babylon::Can_Comb_Class(0);
        }

        Bidir_Props bidir_props(const UCS4 uc) const
        {
            if (!is_defined(uc))
                return BIDIR_INVALID;
            return BIDIR_L;
        }

        Char_Decomp decomp_type(const UCS4 uc) const
        {
            if (!is_defined(uc))
                return DECOMP_MAX;
            return Babylon::Char_Decomp(DECOMP_CANONICAL);
        }

        UTF32_string decompose(const UCS4 uc) const
        {
            UTF32_string us;
            us.resize(1); us[0] = uc;
            return us;
        }

        bool must_mirror(const UCS4 uc) const
        {
            return 0;
        }

        Line_Break linebreak(const UCS4 uc) const
        {
            if (!is_defined(uc))
                return LB_MAX;
            return Babylon::Line_Break(LB_AL);
    }

        EA_Width EA_width(const UCS4 uc) const
        {
            if (!is_defined(uc))
                return EA_WIDTH_MAX;
            return Babylon::EA_Width(EA_WIDTH_N);
        }

        UCS4 compose(const UCS4 starter, const UCS4 last)
        {
            return 0;
        }

        bool exclude_from_composition(const UCS4 uc) const
        {
            return 0;
        }

        bool is_White_Space(const UCS4 uc) const
        {
            return 0;
        }

        bool is_Bidi_Control(const UCS4 uc) const
        {
            return 0;
        }

        bool is_Join_Control(const UCS4 uc) const
        {
            return 0;
        }

        bool is_Dash(const UCS4 uc) const
        {
            return 0;
        }

        bool is_Hyphen(const UCS4 uc) const
        {
            return 0;
        }

        bool is_Quotation_Mark(const UCS4 uc) const
        {
            return 0;
        }

        bool is_Terminal_Punctuation(const UCS4 uc) const
        {
            return my_Terminal_Punctuation.test(uc - my_first_letter);
        }

        bool is_Other_Math(const UCS4 uc) const
        {
            return 0;
        }

        bool is_Hex_Digit(const UCS4 uc) const
        {
            return 0;
        }

        bool is_ASCII_Hex_Digit(const UCS4 uc) const
        {
            return 0;
        }

        bool is_Other_Alphabetic(const UCS4 uc) const
        {
            return my_Other_Alphabetic.test(uc - my_first_letter);
        }

        bool is_Ideographic(const UCS4 uc) const
        {
            return 0;
        }

        bool is_Diacritic(const UCS4 uc) const
        {
            return 0;
        }

        bool is_Extender(const UCS4 uc) const
        {
            return 0;
        }

        bool is_Other_Lowercase(const UCS4 uc) const
        {
            return 0;
        }

        bool is_Other_Uppercase(const UCS4 uc) const
        {
            return 0;
        }

        bool is_Noncharacter_Code_Point(const UCS4 uc) const
        {
            return 0;
        }

        bool is_Other_Grapheme_Extend(const UCS4 uc) const
        {
            return 0;
        }

        bool is_Grapheme_Link(const UCS4 uc) const
        {
            return 0;
        }

        bool is_IDS_Binary_Operator(const UCS4 uc) const
        {
            return 0;
        }

        bool is_IDS_Trinary_Operator(const UCS4 uc) const
        {
            return 0;
        }

        bool is_Radical(const UCS4 uc) const
        {
            return 0;
        }

        bool is_Unified_Ideograph(const UCS4 uc) const
        {
            return 0;
        }

        bool is_Other_Default_Ignorable_Code_Point(const UCS4 uc) const
        {
            return 0;
        }

        bool is_Deprecated(const UCS4 uc) const
        {
            return 0;
        }

        bool is_Soft_Dotted(const UCS4 uc) const
        {
            return 0;
        }

        bool is_Logical_Order_Exception(const UCS4 uc) const
        {
            return 0;
        }

        bool is_Other_ID_Start(const UCS4 uc) const
        {
            return 0;
        }

        bool is_XID_Start_Closure(const UCS4 uc) const
        {
            return 0;
        }

        bool is_XID_Continue_Closure(const UCS4 uc) const
        {
            return 0;
        }

      private:
        // functions
        Runic16A0(const Runic16A0 &) ; // no implementaion!

    // members
        Babylon::UCS4 my_first_letter;
        Babylon::UCS4 my_last_letter;
        // Babylon::UCS4_string my_version;
        static const std::bitset<96> my_is_defined;
        static const unsigned char my_cat[96];
        static const std::bitset<96> my_Terminal_Punctuation;
        static const std::bitset<96> my_Other_Alphabetic;
    }; // class Runic16A0

    const std::bitset<96> Runic16A0::my_is_defined(std::string("000000000000000111111111111111111111111111111111111111111111111111111111111111111111111111111111"));

    const unsigned char Runic16A0::my_cat[] =
    {
        CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, 
        CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, 
        CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, 
        CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, 
        CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, 
        CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, 
        CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, 
        CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, 
        CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, 
        CAT_Lo, CAT_Lo, CAT_Lo, CAT_Po, CAT_Po, CAT_Po, CAT_Nl, CAT_Nl, 
        CAT_Nl, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, 
        CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo, CAT_Lo
    };

    const std::bitset<96> Runic16A0::my_Terminal_Punctuation(std::string("000000000000000000111000000000000000000000000000000000000000000000000000000000000000000000000000"));

    const std::bitset<96> Runic16A0::my_Other_Alphabetic(std::string("000000000000000111000000000000000000000000000000000000000000000000000000000000000000000000000000"));

  }; // namespace
}; // namespace

dload(Babylon::Module::Runic16A0);
