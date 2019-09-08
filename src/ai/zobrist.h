/*****************************************************************************
 * Copyright (C) 2019 MillGame authors
 *
 * Authors: Calcitem <calcitem@outlook.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include <cstdint>

#include "millgame.h"

static const hash_t zobrist0[Board::N_POINTS][POINT_TYPE_COUNT] = {
#ifdef HASH_MAP_CUTDOWN
    {0x4E421A00, 0x3962FF00, 0x6DB6EE00, 0x219AE100},
    {0x1F3DE200, 0xD9AACB00, 0xD5173300, 0xD3F9EA00},
    {0xF5A7BB00, 0xDC410900, 0xEE431900, 0x7CDA7A00},
    {0xFD7B4D00, 0x4138BE00, 0xCCBB2D00, 0xDA609700},
    {0x06D82700, 0xCBC16C00, 0x46F12500, 0xE29F2200},
    {0xCAAB9400, 0x5B02DB00, 0x877CD600, 0x35E43800},
    {0x49FDAE00, 0xE6831400, 0xBE166400, 0x1F49D300},
    {0x50F5B100, 0x149AAF00, 0xF509B900, 0x47AEB500},
    {0x18E99300, 0x76BB4F00, 0xFE173900, 0xF87B8700},
    {0x0A8CD200, 0x630C6B00, 0x88F5B400, 0x0A583E00},
    {0xA0128E00, 0x6F225100, 0x51E99D00, 0x6D35BF00},
    {0x66D6D900, 0x87D36600, 0x75A57A00, 0x534FC400},
    {0x1FE34B00, 0xAD6FB000, 0xE5679D00, 0xF88AFF00},
    {0x0462DA00, 0x4BDE9600, 0xF2891200, 0x10537E00},
    {0x26D8EA00, 0x37E6E700, 0x0871D900, 0xCD5F4F00},
    {0xF4AFA100, 0x44A51B00, 0x77265600, 0x8B796500},
    {0xD8F17D00, 0x80F3D700, 0x6B620600, 0x19B8BB00},
    {0xFBC22900, 0x0FCAB400, 0xFD737400, 0xA647B900},
    {0x296A8D00, 0xA3D74200, 0x624D6D00, 0x459FD400},
    {0xCE8C2600, 0x96544800, 0x41017100, 0x1EDD7A00},
    {0x1FCF9500, 0xA5634E00, 0x21976A00, 0x32902D00},
    {0x55A27C00, 0x49EC5F00, 0x0176A100, 0xCAAAEF00},
    {0x14588600, 0xB4C80800, 0x0153EE00, 0x7D78DF00},
    {0xE9C3C500, 0x66B7A600, 0x3CD93000, 0xDBBA2300},
    {0xF1984100, 0x6BEFDF00, 0xB979FE00, 0xBA4D0600},
    {0x96AECF00, 0x33B96E00, 0x76A99C00, 0x1B876200},
    {0x747B2000, 0x0DEC2400, 0xA4E63200, 0xBA244200},
    {0x59C91B00, 0x41482D00, 0xF2CD3900, 0x30E9C100},
    {0x6B156D00, 0xC7F19100, 0x012D3600, 0xC66B3600},
    {0x63156000, 0xA891FC00, 0xF6C8AC00, 0xD80B9400},
    {0xF641E900, 0xF164BF00, 0x2DBE4C00, 0xE2A40C00},
    {0x53FA0600, 0x4F311700, 0x0ACA7000, 0x2C72F500},
    {0xC8104700, 0x4B76AE00, 0xEB55C800, 0x0DB6EF00},
    {0x7F57AB00, 0x22D06000, 0x39055400, 0xDE9A4300},
    {0x6583AF00, 0x41D14100, 0x9CBF9200, 0x7E528F00},
    {0x2BEFA100, 0x5C5FDC00, 0x4DDAFA00, 0x7C98A100},
    {0x65A13B00, 0x2953BF00, 0x8769A800, 0xE6DCA100},
    {0xD01A6E00, 0xBCD93500, 0x17565900, 0xAD5A7300},
    {0xB04E7D00, 0x815F5300, 0x12469A00, 0xB2F25C00},
    {0x564E4B00, 0xD1943700, 0xA4F63C00, 0x7169E500}
#else
    {0x618A9CF24E421A00, 0xBA7A364A3962FF00, 0xA4306AD06DB6EE00, 0xBD592807219AE100},
    {0x83E4F70B1F3DE200, 0x5153D8FCD9AACB00, 0x4A996847D5173300, 0x2719CCC6D3F9EA00},
    {0x7AE39BDEF5A7BB00, 0xBCD7D5DEDC410900, 0x5B14285CEE431900, 0x9F721DD87CDA7A00},
    {0x5D9ACD64FD7B4D00, 0x620F60444138BE00, 0x9725301DCCBB2D00, 0x9275D47FDA609700},
    {0xF5EC163506D82700, 0xDBF647FACBC16C00, 0xB520224946F12500, 0xB2889032E29F2200},
    {0x964C65F0CAAB9400, 0x461170C85B02DB00, 0xA886E3A7877CD600, 0x26F1B8EF35E43800},
    {0xF5B97EF849FDAE00, 0xEE7C5D59E6831400, 0x32648EFABE166400, 0x6189EDE91F49D300},
    {0x93CBB24B50F5B100, 0xF0F6C79D149AAF00, 0x3A993B39F509B900, 0x1E5308DE47AEB500},
    {0x2600EE1A18E99300, 0x390B489E76BB4F00, 0x6F3B9027FE173900, 0x095BADF5F87B8700},
    {0x8BEE19670A8CD200, 0x6CF81326630C6B00, 0xADE52B7888F5B400, 0x8D3F6C790A583E00},
    {0xCB53C13BA0128E00, 0x3F72BC2E6F225100, 0xB42ED55551E99D00, 0x4984708B6D35BF00},
    {0x9543165266D6D900, 0xAAD0136987D36600, 0x97D1867575A57A00, 0xB207C471534FC400},
    {0xD2303A381FE34B00, 0x93490C78AD6FB000, 0x87113B18E5679D00, 0x54391F89F88AFF00},
    {0xB6DEDA460462DA00, 0x5185B8464BDE9600, 0x51C69A99F2891200, 0x46774A0A10537E00},
    {0xE006203726D8EA00, 0xA5474E6237E6E700, 0x39AC6AA70871D900, 0x3DEE0C9FCD5F4F00},
    {0xF818EB3AF4AFA100, 0x8F3A441844A51B00, 0x8A25D49677265600, 0xCE06B0CA8B796500},
    {0x626F5F46D8F17D00, 0x944977DB80F3D700, 0x7A227AA66B620600, 0x4DCC135019B8BB00},
    {0x711EC2C8FBC22900, 0xE7BB68800FCAB400, 0xD3955CDAFD737400, 0xE7534419A647B900},
    {0x9FDCA93E296A8D00, 0xFDB2801DA3D74200, 0xE3C38E0C624D6D00, 0x4D69B7E2459FD400},
    {0x5A3A714CCE8C2600, 0x05D969D496544800, 0x34FFB95741017100, 0x9B1A08811EDD7A00},
    {0xC6F613271FCF9500, 0xCD947ECFA5634E00, 0x8DB775C121976A00, 0xD8E8477932902D00},
    {0x1CAD3B5655A27C00, 0x8AC13C7C49EC5F00, 0xBA076D030176A100, 0xAC96DC58CAAAEF00},
    {0xFEEFB93114588600, 0x0E5CCD93B4C80800, 0x9BDB4F0C0153EE00, 0xAEB4F8927D78DF00},
    {0x621E3A9EE9C3C500, 0xDE5AA56E66B7A600, 0x030E97EE3CD93000, 0xE9A79619DBBA2300},
    {0x77B25AEAF1984100, 0xB8E4263C6BEFDF00, 0xCE932447B979FE00, 0xFFEE0A6DBA4D0600},
    {0x241CFD8796AECF00, 0xFE8A5B9C33B96E00, 0xD47296D976A99C00, 0x7AB3259A1B876200},
    {0x7977FD45747B2000, 0x84C2C36A0DEC2400, 0x12CF8CDEA4E63200, 0xC02BE51BBA244200},
    {0xBD78281F59C91B00, 0x5058264241482D00, 0xA79BA355F2CD3900, 0x3274B36F30E9C100},
    {0x751C8B5D6B156D00, 0xB7C8814FC7F19100, 0x11E74CCF012D3600, 0xF58E3A35C66B3600},
    {0xF92812B163156000, 0x6E98FEA1A891FC00, 0x3A00752DF6C8AC00, 0xDE4AC1B9D80B9400},
    {0x1382738DF641E900, 0xF698FD60F164BF00, 0xC1E4F6772DBE4C00, 0x80AD23BCE2A40C00},
    {0x22AD6ADB53FA0600, 0xFB5D2D614F311700, 0x1DDDDF550ACA7000, 0x962A4AD92C72F500},
    {0x46EB4A0AC8104700, 0x140BB5664B76AE00, 0xF5088729EB55C800, 0x148E44E10DB6EF00},
    {0x1623D3EB7F57AB00, 0x6E826D9722D06000, 0x49C2732039055400, 0x0C35E2C5DE9A4300},
    {0x594468826583AF00, 0xE190283B41D14100, 0xEA3D0B0A9CBF9200, 0x36BDEA707E528F00},
    {0x4FE884872BEFA100, 0xE70A0AB95C5FDC00, 0xA8EE1E864DDAFA00, 0xDD58D6957C98A100},
    {0xFD678C8865A13B00, 0xFF15F6332953BF00, 0xDCE23A318769A800, 0xDF4C292EE6DCA100},
    {0xFD34EA18D01A6E00, 0xFA1300F7BCD93500, 0xAAC5CC6817565900, 0xE0C64BA5AD5A7300},
    {0x5ECF7987B04E7D00, 0xAB38FFE6815F5300, 0x94EA1A1812469A00, 0x20EDFF94B2F25C00},
    {0x0B2D4606564E4B00, 0x83381E3CD1943700, 0xD3DB04A0A4F63C00, 0x789C60EF7169E500}
#endif /* HASH_MAP_CUTDOWN */
};

#ifdef ONLY_USED_FOR_CONVERT
int main(void)
{
    for (int i = 0; i < 40; i++) {
        printf("{");
        for (int j = 0; j < 3; j++) {
            printf("0x%08X, ", (uint32_t)arr[i][j]);
        }
        printf("0x%08X},\n", (uint32_t)arr[i][3]);
    }

    return 0;
}
#endif
