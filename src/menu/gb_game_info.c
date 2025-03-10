#include <stdio.h>
#include <string.h>  // Include this header for string functions like strstr
#include <stdint.h>
#include "gb_game_info.h"


/* README : to switch between testing with internal image sprites and SD : 
    1) CTRL + F
    2) REPLACE mode
    3.1) to switch to internal : "sd:/aio/assets /    -->     " /
    3.2) to switch to internal : " /    -->     "sd:/aio/assets /

    DON'T forget to change the MAKEFILE to add the sprites !!
*/

GameInfo games_info[] = {
    {
        "b4abb29a67c3351eaf60be9d89320fc927937618",
        "Aladdin",
        "(Europe) (SGB)",
        "DMG-ALAP",
        675043511,
        262144,
        "En",
        "Europe",
        " "
    },
    {
        "c331c0bc8f7ea83fe6972524483f91d8be01f8b0",
        "Back to Earth 3D",
        "(Europe) (Demo)",
        " ",
        3559216662,
        262144,
        "En",
        "Europe",
        " "
    },
    {
        "46ee084745de1bc4e8380ca6adff56572dcdb719",
        "Bill & Ted's Excellent Game Boy Adventure - A Bogus Journey!",
        "(USA, Europe)",
        "DMG-LBE",
        1586455914,
        131072,
        "En",
        "USA, Europe",
        "/bill_ted_64.sprite"
    },
    {
        "6a0ab660fd111c6b6ca75171f858d9570bf11e0c",
        "Game Boy Aging Cartridge",
        "(World) (En,Ja) (v1.1)",
        " ",
        1303591000,
        32768,
        "En,Ja",
        "World",
        "/GB_aging_64.sprite"
    },
    {
        "7db6440ff34ea8dd20eb2628a554dcc53f2cba42",
        "Game Boy Controller Kensa Cartridge",
        "(Japan)",
        " ",
        4117069774,
        32768,
        "Ja",
        "Japan",
        "/GB_Kensa_64.sprite" 
    },
    {
        "c18cced2e3c5b4fbd09f0f927c324d6f569b8e67",
        "Gift",
        "(Europe) (Sample)",
        " ",
        3638777786,
        1048576,
        "En",
        "Europe",
        "/hook_64.sprite" 
    },
    {
        "7cd3c6cc057fd6cfbac174412f082f55271f4997",
        "Hook",
        "(USA) (Sample)",
        " ",
        3230772168,
        131072,
        "En",
        "USA"
   , " " },
    {
        "dc52b66850f63b354f5aaec225902be58923f89a",
        "Humans, The",
        "(USA) (QUByte Classics)",
        " ",
        1166077064,
        262144,
        "En",
        "USA",
        "/the_humans_64.sprite" 
    },
    {
        "a8e6ef37e84a9190a7c23244f98379fd2488db5a",
        "M&M's Minis Madness",
        "(USA) (Sample)",
        " ",
        2200517403,
        1048576,
        "En",
        "USA", 
        "/mms_mini_madness_64.sprite" 
    },
    {
        "5602328e5f340b77a0c62676164a5e351a9acd4f",
        "Mega Man III",
        "(USA) (Sample)",
        " ",
        2197078165,
        262144,
        "En",
        "USA",
        "/mega_man_3_64.sprite" 
    },
    {
        "0b6c999591622a30783a779cd2d642a368a1af5d",
        "Ninja JaJaMaru - The Great World Adventure DX",
        "(USA, Europe) (Ninja JaJaMaru Retro Collection) (Switch)",
        " ",
        3048164517,
        262144,
        " ",
        "USA, Europe" 
   , " " },
    {
        "d4a5436441a03c6474878e3f131a1ac58c7fa030",
        "Outburst",
        "(Japan) (Demo)",
        " ",
        2553806764,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "296c39c40bdc6061bff718a468b79e7e5f46553e",
        "Pocket Camera - Debug Game Tester - Second Impact",
        "(Japan) (Test Program) (v10.24) (SGB)",
        " ",
        359820684,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "ed08371c977511b4953cff43f49a98c66fbed58c",
        "Pocket Monsters Gin",
        "(Japan) (Demo) (Spaceworld 1997) (SGB)",
        " ",
        1917196042,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "97b42db14c43f196563b155fce5d23b170577173",
        "Pocket Monsters Kin",
        "(Japan) (Demo) (Spaceworld 1997) (SGB)",
        " ",
        2884489919,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "423f3176f930ba2722c5b3834d9c6ca3849f6ed0",
        "Populous",
        "(Europe) (Sample)",
        " ",
        3975339375,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "9d7f891afb2ae348529f4316af085b6f672f9c98",
        "Resident Evil",
        "(Unknown) (Demo) (Climax Entertainment)",
        " ",
        445780069,
        524288,
        "En",
        "Unknown"
   , " " },
    {
        "9207cec9760d074afdf9eacf50f87267d51d04be",
        "Rumble & Tumble",
        "(USA) (Demo) (E3 2001)",
        " ",
        1140869485,
        131072,
        "En",
        "USA"
   , " " },
    {
        "32ccbcbd4edd7cd5a391f0b85b9dde79b427cff7",
        "SGB Test Program",
        "(World) (1994.4) (Demo) (SGB)",
        " ",
        2304814109,
        49152,
        "En",
        "World"
   , " " },
    {
        "81fc6bc03e7e094ba544d8d9c1f684271939865b",
        "Shantae",
        "(World) (GBA Enhanced) (Switch)",
        " ",
        519699051,
        4194304,
        "En",
        "World"
   , " " },
    {
        "90fe351921a4c97c0dcad75975a0389c4fade776",
        "Shantae",
        "(USA)",
        "CGB-B3AE",
        3918837147,
        4194304,
        "En",
        "USA"
   , " " },
    {
        "2b031b97b7e37bf57b3630adb5b0ba89dbf70dd1",
        "Shin Nihon Pro Wrestling - Toukon Sanjuushi",
        "(Japan) (Sample)",
        " ",
        316242111,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "781281969c4b98e4ebd7a4e54badb0258d2432e8",
        "Test Drive 6",
        "(Europe) (En,Fr,De,Es,It) (Sample) (GB Comp.)",
        " ",
        790073413,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "70d6c1b7b0dab7f6dec9dbc4eae454a19a37d6cd",
        "Trip World DX",
        "(USA) (Limited Run Games)",
        " ",
        2937566187,
        524288,
        "En",
        "USA"
   , " " },
    {
        "bdf78e28adb73b3bec1c232bb5102cd9cb6128c5",
        "Tutty",
        "(Europe) (Demo)",
        " ",
        3294977802,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "67144a8ca361bf230d79c66501cc13a42e8cf67d",
        "Xtreme Sports",
        "(USA)",
        "CGB-BXGE",
        427984721,
        4194304,
        "En",
        "USA"
   , " " },
    {
        "affa8b29cfe1addebb6b436566dc5eeb4fce04e3",
        "Zelda no Densetsu - Yume o Miru Shima",
        "(Japan) (Rev 1) (Demo) (Special Version)",
        " ",
        3166706074,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "4c6867774bf7936969ffcae71511a1546df15490",
        "Zelda no Densetsu - Yume o Miru Shima DX",
        "(Japan) (Rev 1) (Demo) (Special Version 2) (GB Comp.)",
        " ",
        3859812658,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "49127b4b99e43568ae9d83089c57fc81068f9654",
        "Mani 4 in 1 - Tetris + Alleyway + Yakuman + Tennis",
        "(China) (Ja)",
        "DMG-601",
        205039477,
        262144,
        "Ja",
        "China"
   , " " },
    {
        "4d52cbd2286075f75851d0bdef07c81bd829caae",
        "Mani 4 in 1 - Bubble Bobble + Elevator Action + Chase H.Q. + Sagaia",
        "(China) (En)",
        "DMG-602",
        1543257845,
        524288,
        "En",
        "China"
   , " " },
    {
        "210100e408bb421b7189df6af94e8ed8d5ec6f77",
        "Mani 4 in 1 - Genki Bakuhatsu Gambaruger + Zettai Muteki Raijin-Oh + Zoids Densetsu + Miracle Adventure of Esparks - Ushinawareta Seiseki Perivron",
        "(China) (Ja)",
        "DMG-603",
        3279137801,
        524288,
        "Ja",
        "China"
   , " " },
    {
        "ea6b7fa881e6c1fd80232893e337d28dffa83946",
        "Mani 4 in 1 - R-Type II + Saigo no Nindou + Ganso!! Yancha Maru + Shisenshou - Match-Mania",
        "(China) (Ja)",
        "DMG-604",
        3410540240,
        524288,
        "Ja",
        "China"
   , " " },
    {
        "8b7f0f948f4e334c4fdb319441e5542be8b3e88a",
        "Mani 4 in 1 - Takahashi Meijin no Bouken-jima II + GB Genjin + Bomber Boy + Milon no Meikyuu Kumikyoku",
        "(China) (Ja)",
        "DMG-605",
        2500293614,
        1048576,
        "Ja",
        "China"
   , " " },
    {
        "f7879241824719cb482694b305d20ce6a7a75b3d",
        "Aretha",
        "(Japan)",
        "DMG-A1J",
        498488905,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "ded52facce8c7eeb28f78ed37772dc5c25765574",
        "Daikatana GB",
        "(Japan) (NP)",
        "CGB-A22J",
        732961453,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "e23d98a3339c5e998d763d1fdf692b2e7505ce86",
        "John Romero's Daikatana",
        "(Europe) (En,Fr,It)",
        "CGB-A22X",
        3641060937,
        1048576,
        "En,Fr,It",
        "Europe"
   , " " },
    {
        "fd6fefdd44872f08b5a5a62711391c7e0c91e1de",
        "John Romero's Daikatana",
        "(Europe) (Fr,De,Es)",
        "CGB-A22Y",
        4159189779,
        1048576,
        "Fr,De,Es",
        "Europe"
   , " " },
    {
        "3895e6bc8d47459d5291eec0449b9542538507b6",
        "Majokko Mari-chan no Kisekae Monogatari",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-A23J",
        2471447306,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "98fc7c238329f0644746993802fd8a3e61917a17",
        "Majokko Mari-chan no Kisekae Monogatari",
        "(Japan) (Rev 1) (SGB) (GB Comp.)",
        "DMG-A23J",
        219587909,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "f2c05603f82c4adc5e98832aac08fced002e253a",
        "Pocket Color Mahjong",
        "(Japan) (GB Comp.)",
        "DMG-A2AJ",
        148638452,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "dba5fa6b1275fcd0201cb11acc5d30d44f23b211",
        "Super Black Bass",
        "(USA)",
        "DMG-A2BE",
        1799977026,
        524288,
        "En",
        "USA"
   , " " },
    {
        "17e9f742a0706882c83aee1d7587dc56b9192956",
        "Kanji Boy 2",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-A2CJ",
        2180876568,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "20f1cdbec4a5698b84cfd86736bf8f548178e3d8",
        "Dino Breeder 2",
        "(Japan) (SGB)",
        "DMG-A2DJ",
        94612346,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "c8e742a4d99d3a44704611979a16149e6afc4e59",
        "Beatmania GB 2 - Gotcha Mix",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-A2GJ",
        1505670002,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "fe997f9ef5463710e525917d0670d8c617fa2bb8",
        "Shounen Ashibe - Yuuenchi Panic",
        "(Japan)",
        "DMG-A2J",
        3003530677,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "07e7e54d552e28770e55e14acf014a103e9706c5",
        "Jurassic Park Part 2 - The Chaos Continues",
        "(USA, Europe) (En,Fr,De,It)",
        "DMG-A2JE",
        2940022421,
        262144,
        "En,Fr,De,It",
        "USA, Europe"
   , " " },
    {
        "0eab6c16d00424bc6b36740128e7cfa35c250079",
        "Konchuu Hakase 2",
        "(Japan) (Rev 1) (SGB) (GB Comp.)",
        "DMG-A2KJ",
        3549446926,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "5b915716d6c5d487cb1d898aa81c1d5570d25db9",
        "Konchuu Hakase 2",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-A2KJ",
        3057129284,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "8ea94737f4b1ae96bfbf2e20f2fe680e23627623",
        "Megami Tensei Gaiden - Last Bible II",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-A2LJ",
        2628386997,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "a3cd78cf32d0b21b1dd388f59c5b05fd1f53e2f1",
        "Medarot 2 - Kabuto Version",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-A2MJ",
        3211514950,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "8d76f672324394eb53e893bd694d000bd6ed34e5",
        "Medarot 2 - Kuwagata Version",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-A2NJ",
        1536163127,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "94e7b93b858b5293f231ad4ed42498478f616c89",
        "Super Bombliss DX",
        "(Japan) (En) (SGB) (GB Comp.)",
        "DMG-A2OJ",
        885564581,
        262144,
        "En",
        "Japan"
   , " " },
    {
        "debb5e3507e6f41dc1572be7dc420886398163a7",
        "Purikura Pocket 2 - Kareshi Kaizou Daisakusen",
        "(Japan) (SGB)",
        "DMG-A2PJ",
        3511020067,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "e9ce5756ab1d299a48a74b52e15d5fe124c5552a",
        "RPG Tsukuru GB",
        "(Japan)",
        "CGB-A2QJ",
        190923527,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "cc4b0d991eb7812525ad03f81fbdb7593daffa8c",
        "Mighty Morphin Power Rangers - The Movie",
        "(USA, Europe) (SGB)",
        "DMG-A2RE",
        2258748745,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "7c91b7eb5d8521e5dc62a26a257b29787b3ff640",
        "Chalvo 55 - Super Puzzle Action",
        "(Japan)",
        "DMG-A2SJ",
        1960119111,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "bf0f80737a794a9e57a82efc4b2e5f7434f8de15",
        "Turok 2 - Seeds of Evil",
        "(USA, Europe) (En,Fr,De,Es) (GB Comp.)",
        "DMG-A2TE",
        1859807802,
        1048576,
        "En,Fr,De,Es",
        "USA, Europe"
   , " " },
    {
        "710dca14b11d5f0ab2adc6847b037069808f31ac",
        "Turok 2 - Jikku Senshi",
        "(Japan) (GB Comp.)",
        "DMG-A2TJ",
        4036342854,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "d9acd50c819c4f3b09d59ce1db747c31b2f10130",
        "Kakutou Ryouri Densetsu Bistro Recipe - Kettou Bistgarm Hen",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-A2WJ",
        1337902180,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "ae0198644e9a5ffa246a42130930181f13fba031",
        "Micro Machines 2 - Turbo Tournament",
        "(Europe)",
        "DMG-A2XP",
        610692173,
        524288,
        "En",
        "Europe"
   , " " },
    {
        "fd27528acfd14465bdd8063fde0ca893ce932ec9",
        "Top Gear Pocket 2",
        "(USA)",
        "CGB-A33E",
        4021845888,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "cb00a9b3570f1cf4f53585136b69956151c2e93e",
        "Top Gear Rally 2",
        "(Europe)",
        "CGB-A33P",
        24605645,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "dc35c1a2a773e0c06c3f69bfdf01156e5490818a",
        "TG Rally 2",
        "(United Kingdom)",
        "CGB-A33X",
        2035980690,
        1048576,
        "En",
        "United Kingdom"
   , " " },
    {
        "2d21c8358547f1919a14bed34e683b131f9d48b5",
        "Moomin no Daibouken",
        "(Japan)",
        "CGB-A36J",
        3173641839,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "045af60ab652f4c3dea4e53b44d72abbe9025962",
        "Bokujou Monogatari 2 GB",
        "(Japan) (GB Comp.)",
        "DMG-A37J",
        4003017474,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "288700f9dc29be1c8274960fd41552e1a022ce6e",
        "Pocket Color Billiards",
        "(Japan)",
        "CGB-A3AJ",
        2433746913,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "31da0b20640478e561b1e0627b6638207375b7b2",
        "Batman Forever",
        "(USA, Europe)",
        "DMG-A3BE",
        4013265612,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "9995c5ab23e9bf6ef067fdfe62c323037f7dee1b",
        "Batman Forever",
        "(Japan)",
        "DMG-A3BJ",
        3271385668,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "6d4e0e588181704930a46b61556cd12924645d99",
        "Catwoman",
        "(USA)",
        "CGB-A3CE",
        317768086,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "80796aa43ac74aa2a545e0b008c5a4a7283bae62",
        "Catwoman",
        "(Europe)",
        "CGB-A3CP",
        1105692229,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "4ebc5fb7fff8b57a5049c24111679f9e2cb53a52",
        "Dino Breeder 3 - Gaia Fukkatsu",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-A3DJ",
        3656413627,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "f75fa2f8983e6d03fa72454d03342703da863b1e",
        "PGA Tour 96",
        "(USA, Europe) (SGB)",
        "DMG-A3GE",
        1878466627,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "9ddda93f7d4be3d60a1e19fc368beb3526597d32",
        "Millennium Winter Sports",
        "(USA)",
        "CGB-A3HE",
        3044912046,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "658420c7fa442a5a789a363a4ded4e014e801c14",
        "Hyper Olympic - Winter 2000",
        "(Japan)",
        "CGB-A3HJ",
        2847469682,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "9f9f24053e9e4a713eeba6faf5f0a6eeeeda8521",
        "Konami Winter Games",
        "(Europe)",
        "CGB-A3HP",
        476332096,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "57b524549fe271bd703dd0d31b66d5a140852f0a",
        "Chachamaru Boukenki 3 - Abyss no Tou",
        "(Japan)",
        "DMG-A3J",
        2650622912,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "0c584db9687316d0a2e2d9089cee2672feeeb885",
        "Konchuu Hakase 3",
        "(Japan)",
        "CGB-A3KJ",
        940513773,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "1be21bca712ddab2a667a6d226b70c8b87d22b9e",
        "Mortal Kombat 3",
        "(USA, Europe)",
        "DMG-A3ME",
        3214697979,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "6288911c57ae87f5f609093f187b4ae00c2abc83",
        "Mortal Kombat 3",
        "(Europe)",
        "DMG-A3MP",
        1086968050,
        524288,
        "En",
        "Europe"
   , " " },
    {
        "b98323d46ab35486ebe4e1a44567784e2bf3d13d",
        "Purikura Pocket 3 - Talent Debut Daisakusen",
        "(Japan) (Rev 1) (SGB) (NP)",
        "DMG-A3PJ",
        1166921821,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "f1fa616f29c0cc2e5098006870046bb825089440",
        "Purikura Pocket 3 - Talent Debut Daisakusen",
        "(Japan) (SGB)",
        "DMG-A3PJ",
        3468043281,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "4525b1d3db34ac5d40987c659788ca06315bd973",
        "Daffy Duck - Subette Koron de Oogane Mochi",
        "(Japan) (GB Comp.)",
        "DMG-A3SJ",
        1531250649,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "9deb521deb95d3eae7da1e6e2c739aae21c6dd88",
        "Bust-A-Move 3 DX",
        "(Europe)",
        "DMG-A3VE",
        3847603730,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "b54a0e83d6477b7d61f26866e9bbdea94363e43b",
        "Animal Breeder 4",
        "(Japan)",
        "CGB-A4AJ",
        2211985603,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "45989b2b9a9507206e88bfd5b4a91191453ac6df",
        "Meitantei Conan - Karakuri Jiin Satsujin Jiken",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-A4BJ",
        996091132,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "47c73b5f5db662ef6905f81282a28cc677501950",
        "Pocket Color Trump",
        "(Japan) (GB Comp.)",
        "DMG-A4CJ",
        3342774542,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "71749f4cd6d91bf80862a928cdb0452693993dc5",
        "Dino Breeder 4",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-A4DJ",
        2953865079,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "0f76ad057bf8110a05639460b5471fdb1482fb0d",
        "Aretha II",
        "(Japan)",
        "DMG-A4J",
        141447109,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "bbfce367910bdf1b145fc951280046e045b265c0",
        "Mortal Kombat 4",
        "(Germany) (En) (SGB) (GB Comp.)",
        "DMG-A4KD",
        2270725925,
        1048576,
        "En",
        "Germany"
   , " " },
    {
        "8a3b63c908e8444b40f963eb89e2853fe6cdbfd2",
        "Mortal Kombat 4",
        "(USA, Europe) (SGB) (GB Comp.)",
        "DMG-A4KE",
        1320621128,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "47d4f3efcfd87ce9d8bed9bff12e8ffb1b50d6ca",
        "Medarot - Parts Collection 2",
        "(Japan) (SGB)",
        "DMG-A4MJ",
        2314814594,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "70afd945e6d659739154714ca2f14d8576e3eede",
        "Bakukyuu Renpatsu!! Super B-Daman - Gekitan! Rising Valkyrie!!",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-A4RJ",
        1553323426,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "13fdf7df92fcf309f5f07735f0205db0b31baf9d",
        "Joryuu Janshi ni Chousen GB - Watashi-tachi ni Chousen Shitene!",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-A55J",
        2678443445,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "ff811fd66fc3014552cbd818a6420b4cbb36d80a",
        "Mahjong Joou",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-A56J",
        2105779688,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "c0fd56646f965593761b617881d7e55cb52adbf2",
        "Meitantei Conan - Kigantou Hihou Densetsu",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-A5BJ",
        3860921058,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "f2ba5e428bdacad7def0dfc080c0f5bf19e1ba2a",
        "Keibajou e Ikou! Wide",
        "(Japan) (GB Comp.)",
        "DMG-A5CJ",
        1895991018,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "f5dd6addfc56d3216ec9fe814772e2c33e563877",
        "Kinniku Banzuke GB - Chousensha wa Kimida!",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-A5KJ",
        3806605963,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "ea554325cc4ce4698e0302b63790ea085822919b",
        "MetaFight EX",
        "(Japan)",
        "CGB-A5MJ",
        7552119,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "79ed823a1e224abdc814202d257cd72f0e5fc9d2",
        "Indien dans la Ville, Un",
        "(Europe) (SGB)",
        "DMG-A65P",
        3816691762,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "6af14af436c0500ae6a8d74e64ac5b62c881ad8d",
        "Super Chinese Fighter EX",
        "(Japan)",
        "CGB-A66J",
        3705316147,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "b99504cc5788c48a37f261ddbfe4aaad49f5ce39",
        "NBA Live 96",
        "(USA, Europe) (SGB)",
        "DMG-A6BE",
        2290468813,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "0182a9849a0e00ae6c96354543c674b01253bfd2",
        "We're Back! - A Dinosaur's Story",
        "(USA, Europe)",
        "DMG-A6E",
        3356579339,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "03cdf147bc1381fe86e091167d3d44379b4006f2",
        "NHL 96",
        "(USA, Europe) (SGB)",
        "DMG-A6HE",
        365767523,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "cb6fd0608f70d368291042fab0b93f9fd3e2f133",
        "Madden 96",
        "(USA, Europe) (SGB)",
        "DMG-A6ME",
        4254974381,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "686d9b79626319e1e214a03e195f844e9e7f2a15",
        "Pocket Hanafuda",
        "(Japan)",
        "CGB-A6PJ",
        2936820152,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "73be85504a98d7713abbeb5d5d2d1d7480341d73",
        "FIFA Soccer 96",
        "(USA, Europe) (En,Fr,De,Es) (SGB)",
        "DMG-A6SE",
        2940886497,
        524288,
        "En,Fr,De,Es",
        "USA, Europe"
   , " " },
    {
        "8fa1a66fe511c791c7dab84328ac42a762041c44",
        "Monster Rancher Battle Card GB",
        "(USA) (SGB) (GB Comp.)",
        "DMG-A6TE",
        1356722464,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "4f84481d0fa74f53f415e7d8f9a4c9387667833e",
        "Monster Farm Battle Card GB",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-A6TJ",
        1773702942,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "22e8e8b6f4898cb9418be234e79043b722fb858a",
        "Bamse",
        "(Sweden)",
        "DMG-A6W",
        460406240,
        131072,
        "Sv",
        "Sweden"
   , " " },
    {
        "6ec068a8efcfc694739fccb25776a0100964e920",
        "Agro Soar",
        "(Australia)",
        "DMG-A6X",
        2608416234,
        131072,
        "En",
        "Australia"
   , " " },
    {
        "f276080d7afc0fdc34d00c22922cc9d4c0d81685",
        "Baby T-Rex",
        "(Europe)",
        "DMG-A6Y",
        1064566394,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "47262e59e0d29741a309a64eb20054a10bc9a957",
        "Densha de Go!",
        "(Japan)",
        "CGB-A72J",
        339169394,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "a03a97c91a86b598b42a5e26833521c840b0485f",
        "Bomberman Max - Red Challenger",
        "(USA)",
        "CGB-A7BE",
        1213461894,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "98f83e660e53d242f4ea423dce355b9ead9135fe",
        "Bomberman Max - Yami no Senshi",
        "(Japan)",
        "CGB-A7BJ",
        1219890830,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "db1c0ce5777a6d29cf3f4b846490e63d0221f8b0",
        "Medarot 2 - Parts Collection",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-A7CJ",
        362181595,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "511776254cbfa4cf1034d254f61f0ed833f2b644",
        "Pocket GT",
        "(Japan)",
        "CGB-A7GJ",
        717716570,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "74-25310",
        "Aa Harimanada",
        "(Japan)",
        "DMG-A7J",
        1543490600,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "3678d8756eb1be746ec230003dbd82722548356d",
        "Kawa no Nushi Tsuri 3",
        "(Japan) (SGB)",
        "DMG-A7KJ",
        343835310,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "2f73586b27b211478aeb05f3bdcb2d4569f4b705",
        "Madden 97",
        "(USA) (SGB)",
        "DMG-A7ME",
        2292725666,
        524288,
        "En",
        "USA"
   , " " },
    {
        "356350997e74d45d81c1d8fd0568a051a19ad52d",
        "Granduel - Shinki Dungeon no Hihou",
        "(Japan) (Sample)",
        "CGB-A7PJ",
        1898058605,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "98ad7bb8536c4ddfc4cc7761efad8c2e67195175",
        "FIFA Soccer 97",
        "(USA, Europe) (SGB)",
        "DMG-A7SE",
        3702231148,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "5296c29a4b03d716d6584bbab8a04a27471b0b32",
        "Minna no Shougi - Shokyuu Hen",
        "(Japan) (GB Comp.)",
        "DMG-A7YJ",
        1555365002,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "6493860f4a13db8f7a6f59de223b0e4617828d7b",
        "Minna no Shougi - Shokyuu Hen",
        "(Japan) (Rev 1) (GB Comp.)",
        "DMG-A7YJ",
        4150657370,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "6ceaeb9ca4c1d09eded0f4efc4c32bc29e6e0b53",
        "Data-Navi Pro Yakyuu",
        "(Japan)",
        "CGB-A89J",
        3378031989,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "2e2aa202523ad44e1efd8c07b71c7b08b09adcc4",
        "Bomberman Max - Blue Champion",
        "(USA)",
        "CGB-A8BE",
        1556833999,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "b0d2a01307e2a193354237fd634ebad4b90691e0",
        "Bomberman Max - Hikari no Yuusha",
        "(Japan)",
        "CGB-A8BJ",
        2051329672,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "63548700818518d92443791319215f3c339f2925",
        "Medarot Cardrobottle - Kabuto Version",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-A8CJ",
        1325651978,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "158-15799",
        "Addams Family, The - Pugsley's Scavenger Hunt",
        "(USA, Europe)",
        "DMG-A8E",
        2114275976,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "2c8a9abb366a772e020439fdfb59289cce999e30",
        "Elie no Atelier GB",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-A8EJ",
        2036806158,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "566d5082b140a43be8256ed123a6de771114c92a",
        "Keitai Keiba Eight Special",
        "(Japan)",
        "DMG-A8GJ",
        845312951,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "3639750c9544f7c4ae535f99d8699744d057ab71",
        "Marie no Atelier GB",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-A8MJ",
        1631911014,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "ba2c6486afd9225b1302f12666127c426bb7ab26",
        "NBA Jam",
        "(USA, Europe)",
        "DMG-A8NE",
        211844314,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "c3a26937210cb69dd59258ebb71cd99de971ad15",
        "FIFA - Road to World Cup 98",
        "(Europe) (SGB)",
        "DMG-A8SE",
        3455493548,
        524288,
        "En",
        "Europe"
   , " " },
    {
        "ab96157b2756332b79f845e740c1f494a200902c",
        "FIFA - Road to World Cup 98",
        "(Europe) (Rev 1) (SGB)",
        "DMG-A8SP",
        2378224748,
        524288,
        "En",
        "Europe"
   , " " },
    {
        "00ab156013e2de62519f9799051e2fd199ae1f5b",
        "Umi no Nushi Tsuri 2",
        "(Japan) (SGB)",
        "DMG-A8UJ",
        436165438,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "b8d9cfb891ef608b4d44008fb980708310fe03f1",
        "World Cup 98",
        "(USA, Europe) (SGB)",
        "DMG-A8WE",
        1781691057,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "4fffce2598f4d11eb464f95c2bca272c6fe89444",
        "Hyper Black Bass '95",
        "(Japan) (En,Ja)",
        "DMG-A95J",
        849877577,
        262144,
        "En,Ja",
        "Japan"
   , " " },
    {
        "04ce00460b0ae0fa530627993fbd982c99605be4",
        "Navyblue 98",
        "(Japan)",
        "DMG-A98J",
        750670721,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "8fb81e4c721dda13f50b4932fea26764ccff7a96",
        "Pocket Billiards - Funk the 9 Ball",
        "(Japan)",
        "CGB-A9BJ",
        994502601,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "eb158c30c9f4c8b0937cb0ccc096220982e1787f",
        "Medarot Cardrobottle - Kuwagata Version",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-A9CJ",
        2742247297,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "ff646b4925be49d17103c4a1365e9b7002b863e4",
        "Alien vs Predator - The Last of His Clan",
        "(USA)",
        "DMG-A9E",
        1270675026,
        131072,
        "En",
        "USA"
   , " " },
    {
        "7939b5d23fffd3e73facd998cb347c3f3ac5f751",
        "Alien vs Predator",
        "(Japan)",
        "DMG-A9J",
        3900991531,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "5f51a8519890546cb065dcc250cb1cfb35230131",
        "TV Champion",
        "(Japan) (SGB)",
        "DMG-A9YJ",
        418793337,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "e5f866fef87641ebda6da5eeaeac7012ecaba088",
        "Pro Mahjong Kiwame II GB",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AA2J",
        351870854,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "07fc3a7f701630125257f37f032a9def717f0bf0",
        "Animal Breeder 3",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AA3J",
        3324660784,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "d282d8326c3c0c23be0349237bb8d2c593c03f67",
        "Bust-A-Move 4",
        "(USA, Europe) (GB Comp.)",
        "DMG-AA4E",
        2390855279,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "dcf878fcf8a965b066d1fb744185dfb518d00a41",
        "Puzzle Bobble 4",
        "(Japan) (GB Comp.)",
        "DMG-AA4J",
        2040746628,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "2a3c8b8ab824a14f560a19fa1851a588d146f99c",
        "NBA Show Time - NBA on NBC",
        "(USA)",
        "CGB-AA5E",
        2061645960,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "8587fdd4213613717a9da6b9488154a1a71ac861",
        "Asterix - Sur la Trace D'Idefix",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-AA6P",
        1083033030,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "152-690",
        "720 Degrees",
        "(USA, Europe) (GB Comp.)",
        "DMG-AA7E",
        3862135839,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "e1c2178683491642f62c17f4a68241e276026acf",
        "Honkaku Yonin Uchi Mahjong - Mahjong Ou",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AAAJ",
        2396306362,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "bccc8133ef6a29b77b01cc8042c4773b0c1996a6",
        "Another Bible",
        "(Japan) (SGB)",
        "DMG-AABJ",
        1246258229,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "41880510a77e0e07d5af17174e50611e5137d7e4",
        "Pac-Man - Special Color Edition",
        "(USA) (SGB) (GB Comp.)",
        "DMG-AACE",
        881192838,
        262144,
        "En",
        "USA"
   , " " },
    {
        "f85166e080abe36fe83d64c8d4567ac28c0b1791",
        "Pac-Man - Special Colour Edition",
        "(Europe) (SGB) (GB Comp.)",
        "DMG-AACP",
        4117062783,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "78d85dccbc0a8b5d5dca57a1990af49bb339cfab",
        "Midway Presents Arcade Hits - Joust & Defender",
        "(USA, Europe) (GB Comp.)",
        "DMG-AADE",
        1277087435,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "fc97220413f5b85b44a2df704a20d498dcd48687",
        "Golden Goal",
        "(Germany) (En,Fr,De,Es,It,Nl,Sv) (GB Comp.)",
        "DMG-AAFD",
        349285822,
        1048576,
        "En,Fr,De,Es,It,Nl,Sv",
        "Germany"
   , " " },
    {
        "45e310206cf4dd14242e68c887234440d7c6df11",
        "Pro Foot",
        "(France) (En,Fr,De,Es,It,Nl,Sv) (GB Comp.)",
        "DMG-AAFF",
        2844809522,
        1048576,
        "En,Fr,De,Es,It,Nl,Sv",
        "France"
   , " " },
    {
        "e7c589b8e469d8a98694a370ab68fe96074ad0ae",
        "Three Lions",
        "(United Kingdom) (En,Fr,De,Es,It,Nl,Sv) (GB Comp.)",
        "DMG-AAFP",
        482255274,
        1048576,
        "En,Fr,De,Es,It,Nl,Sv",
        "United Kingdom"
   , " " },
    {
        "25cc2cfe30a3a2f87ce0108a8711f1639e136026",
        "Golden Goal",
        "(Europe) (En,Fr,De,Es,It,Nl,Sv) (GB Comp.)",
        "DMG-AAFX",
        3147520381,
        1048576,
        "En,Fr,De,Es,It,Nl,Sv",
        "Europe"
   , " " },
    {
        "bb0fdf5e9ffc7cbfce5baa693ef72854022755a0",
        "Golf Daisuki!",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AAGJ",
        2151959174,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "797d6d8158ef22c1fad03ec7e621e85eb89e6db4",
        "Ponta to Hinako no Chindouchuu - Yuujou Hen",
        "(Japan)",
        "DMG-AAJ",
        93011371,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "4238a8dc71c030b0d06aac13423297f79663f80a",
        "Animal Breeder 2",
        "(Japan) (SGB)",
        "DMG-AAJJ",
        2371305271,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "5ad321ca3b40d3cb19994e52795b12c2027eff59",
        "Backgammon",
        "(Japan) (GB Comp.)",
        "DMG-AAKJ",
        1898596787,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "a6671051d8222f75526f2fcbfe094a1ff2675186",
        "Backgammon",
        "(Europe) (En,Fr,De,Es) (GB Comp.)",
        "DMG-AAKP",
        3544018811,
        1048576,
        "En,Fr,De,Es",
        "Europe"
   , " " },
    {
        "9655bee4be73a9778531a5522eb83555ceac23be",
        "Aqualife",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AALJ",
        3796106932,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "d4d469991fa7bd9f54c468812c009da68bcdc80f",
        "Mini Yonku GB Let's & Go!! - All-Star Battle Max",
        "(Japan) (SGB)",
        "DMG-AAMJ",
        2548814365,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "ba21301ecd0edce1ae66c43ea573eb0b6468ffed",
        "Soreike! Anpanman - Fushigi na Nikoniko Album",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AANJ",
        2819550893,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "b5e356e81e611961961c2d4e06e8f308317c28ce",
        "Worms Armageddon",
        "(USA) (En,Fr,Es)",
        "CGB-AAOE",
        3795971550,
        1048576,
        "En,Fr,Es",
        "USA"
   , " " },
    {
        "69c567264b3adcd77d187f31aced460ed4f43d2c",
        "Asteroids",
        "(USA, Europe) (GB Comp.)",
        "DMG-AARE",
        2312493366,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "386268778c46009fedae0cde77521cbbb56da0a8",
        "All-Star Baseball 2000",
        "(USA, Europe)",
        "CGB-AATE",
        3783219621,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "76-56471",
        "Pokemon - Goldene Edition",
        "(Germany) (SGB) (GB Comp.)",
        "DMG-AAUD",
        1216995242,
        2097152,
        "De",
        "Germany"
   , "/pokemon_gold64.sprite" },
    {
        "75-26669",
        "Pokemon - Gold Version",
        "(USA, Europe) (SGB) (GB Comp.)",
        "DMG-AAUE",
        1809726526,
        2097152,
        "En",
        "USA, Europe"
   , "/pokemon_gold64.sprite" },
    {
        "74-28614",
        "Pokemon - Version Or",
        "(France) (SGB) (GB Comp.)",
        "DMG-AAUF",
        933693186,
        2097152,
        "Fr",
        "France"
   , "/pokemon_gold64.sprite" },
    {
        "71-52748",
        "Pokemon - Versione Oro",
        "(Italy) (SGB) (GB Comp.)",
        "DMG-AAUI",
        1276660963,
        2097152,
        "It",
        "Italy"
   , "/pokemon_gold64.sprite" },
    {
        "71-33888",
        "Pocket Monsters Kin",
        "(Japan) (Rev 1) (SGB) (GB Comp.)",
        "DMG-AAUJ",
        1324872357,
        1048576,
        "Ja",
        "Japan"
   , "/pokemon_gold64.sprite" },
    {
        "72-35440",
        "Pocket Monsters Kin",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AAUJ",
        1380219092,
        1048576,
        "Ja",
        "Japan"
   , "/pokemon_gold64.sprite" },
    {
        "8-30602",
        "Pocket Monsters Geum",
        "(Korea)",
        "CGB-AAUK",
        614103654,
        2097152,
        "Ko",
        "Korea"
   , "/pokemon_gold64.sprite" },
    {
        "61-37715",
        "Pokemon - Edicion Oro",
        "(Spain) (SGB) (GB Comp.)",
        "DMG-AAUS",
        875866411,
        2097152,
        "Es",
        "Spain"
   , "/pokemon_gold64.sprite" },
    {
        "150d47813580681bd4f83f5417028fada0ec8a3c",
        "Spawn",
        "(USA)",
        "CGB-AAWE",
        1929162925,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "43-52590",
        "Pokemon - Silberne Edition",
        "(Germany) (SGB) (GB Comp.)",
        "DMG-AAXD",
        2529811349,
        2097152,
        "De",
        "Germany",
        "/pokemon_silver64.sprite" 
    },
    {
        "42-3502",
        "Pokemon - Silver Version",
        "(USA, Europe) (SGB) (GB Comp.)",
        "DMG-AAXE",
        2329183798,
        2097152,
        "En",
        "USA, Europe",
        "/pokemon_silver64.sprite" 
    },
    {
        "41-64396",
        "Pokemon - Version Argent",
        "(France) (SGB) (GB Comp.)",
        "DMG-AAXF",
        3770816234,
        2097152,
        "Fr",
        "France",
        "/pokemon_silver64.sprite" 
    },
    {
        "38-29520",
        "Pokemon - Versione Argento",
        "(Italy) (SGB) (GB Comp.)",
        "DMG-AAXI",
        3416707796,
        2097152,
        "It",
        "Italy",
        "/pokemon_silver64.sprite" 
    },
    {
        "38-7476",
        "Pocket Monsters Gin",
        "(Japan) (Rev 1) (SGB) (GB Comp.)",
        "DMG-AAXJ",
        183128963,
        1048576,
        "Ja",
        "Japan",
        "/pokemon_silver64.sprite" 
    },
    {
        "39-30353",
        "Pocket Monsters Gin",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AAXJ",
        3189478026,
        1048576,
        "Ja",
        "Japan",
        "/pokemon_silver64.sprite" 
    },
    {
        "231-39003",
        "Pocket Monsters Eun",
        "(Korea)",
        "CGB-AAXK",
        2629957903,
        2097152,
        "Ko",
        "Korea",
        "/pokemon_silver64.sprite" 
    },
    {
        "28-1611",
        "Pokemon - Edicion Plata",
        "(Spain) (SGB) (GB Comp.)",
        "DMG-AAXS",
        497003205,
        2097152,
        "Es",
        "Spain",
        "/pokemon_silver64.sprite" 
    },
    {
        "d0bae71326b1ef7021ca1e7793e7aeb42721f034",
        "Azure Dreams",
        "(Europe) (En,Fr,De) (SGB) (GB Comp.)",
        "DMG-AAYP",
        4190939913,
        2097152,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "b3c04dd294ba7a953e6cf9998b5db61415a8ebb0",
        "Azure Dreams",
        "(USA) (SGB) (GB Comp.)",
        "DMG-AAZE",
        1909794934,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "648d7857a772fce5b003fe7acbcebdac3cbc6dc3",
        "Other Life - Azure Dreams GB",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AAZJ",
        3337726932,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "726baa1e2484c97ac7f0ce522179fbe390ee4b00",
        "Bomberman GB",
        "(USA, Europe) (SGB)",
        "DMG-AB2E",
        4084388213,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "d7eebdc27623879941a135b5a1e9c1a96fba57fe",
        "Bomberman GB 2",
        "(Japan) (SGB)",
        "DMG-AB2J",
        1633109051,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "3b89f51095d62853fc3b829d0a599ddb887cf6a4",
        "Bomberman GB 3",
        "(Japan) (SGB)",
        "DMG-AB3J",
        4133009319,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "d3998757fa407ce7a4530fa8ad7e4e1a353b961e",
        "Goukaku Boy Series - Koukou Nyuushi Derujun - Chuugaku Eitango 1700",
        "(Japan) (Imagineer)",
        "DMG-AB4J",
        3541470922,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "0d982ab5b26ac8b5a752d7f49f8435b7e89aff32",
        "Bomberman Quest",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AB5J",
        1428228404,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "1f9d27bbe52bfb6067e274aa773f22c2a58155b2",
        "Bugs Bunny - Crazy Castle 3",
        "(USA, Europe) (GB Comp.)",
        "DMG-AB6E",
        2049442299,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "818633a4a871bf664d87df5e8151550337601b22",
        "Bugs Bunny - Crazy Castle 3",
        "(Japan) (GB Comp.)",
        "DMG-AB6J",
        2927860910,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "a0bebbbff45f929d5abc2de00c09c08987ebc96a",
        "Kettou Transformers Beast Wars - Beast Senshi Saikyou Ketteisen",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AB7J",
        1921603129,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "9110c513643335e5fd74c383aa56092855c4f463",
        "Schiffe Versenken",
        "(Germany) (En,Fr,De,Es)",
        "DMG-AB8D",
        481869762,
        131072,
        "En,Fr,De,Es",
        "Germany"
   , " " },
    {
        "929856dd3237813978422dd3f9ce92f9e4a2d805",
        "Bataille Navale",
        "(France) (En,Fr,De,Es)",
        "DMG-AB8F",
        3132667687,
        131072,
        "En,Fr,De,Es",
        "France"
   , " " },
    {
        "23b8d6f28b4cd28e0372b910d1942e8d63fe7866",
        "Sea Battle",
        "(Europe) (En,Fr,De,Es)",
        "DMG-AB8P",
        3121159057,
        131072,
        "En,Fr,De,Es",
        "Europe"
   , " " },
    {
        "a65b90f83291f583b90762e40de2adb90c913384",
        "All-Star Baseball 99",
        "(USA)",
        "DMG-AB9E",
        3575572179,
        524288,
        "En",
        "USA"
   , " " },
    {
        "230-9139",
        "After Burst",
        "(Japan)",
        "DMG-ABA",
        3221527927,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "21e056e982958a3c1f152fdb1ca8c357e0fb50ca",
        "Battle Crusher",
        "(Japan) (SGB)",
        "DMG-ABAJ",
        3931025825,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "2368e515fa5eba68acf266ca23f349095fa7999f",
        "Bubsy II",
        "(USA)",
        "DMG-ABBE",
        1611295445,
        262144,
        "En",
        "USA"
   , " " },
    {
        "5a776e8b37aafd7ac3e6f62d19cfc90c025369fc",
        "Bubsy II",
        "(Europe)",
        "DMG-ABBP",
        2736145686,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "74cbb37163e6cfd8ad9ad9743fc9897ddb771ea3",
        "Bomberman Collection",
        "(Japan) (SGB)",
        "DMG-ABCJ",
        1352297331,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "f50a00a59a567a3d537435d4b777c784255fe7a4",
        "Super B-Daman - Fighting Phoenix",
        "(Japan) (SGB)",
        "DMG-ABDJ",
        597103084,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "0985262dee6f108016f97390a874dc66252cc3a5",
        "Barcode Taisen Bardigun",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-ABEJ",
        3724342455,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "ed7d59b8e3b04ce63de06dac0e17378791bb4912",
        "Barcode Taisen Bardigun",
        "(Japan) (Rev 1) (SGB) (GB Comp.)",
        "DMG-ABEJ",
        4140994985,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "6b9b3b69dbccb030846367135d9ba942925c01c8",
        "Pocket Bass Fishing",
        "(Japan)",
        "DMG-ABFJ",
        747174294,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "ae0318f45278e760aa12bb41325e917a400d32b6",
        "Pocket Mahjong",
        "(Japan)",
        "DMG-ABGJ",
        2431657684,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "4bbdfe8ca27a73ccb6ea76acaa61312f5eca0066",
        "Nada Asatarou & Kojima Takeo no Jissen Mahjong Kyoushitsu",
        "(Japan) (SGB)",
        "DMG-ABIJ",
        2637386082,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "dd8c962e7706f7b68b0c4cc03cac06b032cd0ef0",
        "Soreyuke!! Kid",
        "(Japan)",
        "DMG-ABJJ",
        709504299,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "0b781be45106da220620c890b50b706420b2254b",
        "Block Kuzushi GB",
        "(Japan) (SGB)",
        "DMG-ABKJ",
        1421243649,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "80cbb3d20bc4d7d3da68626a0a18b8dab76c6644",
        "Looney Tunes - Carrot Crazy",
        "(USA) (En,Fr,Es) (GB Comp.)",
        "DMG-ABLE",
        301684247,
        1048576,
        "En,Fr,Es",
        "USA"
   , " " },
    {
        "b6f4939203e0987076203b8f195e4698c8ca57d0",
        "Bugs Bunny & Lola Bunny - Operation Carrot Patch",
        "(Europe) (En,Fr,De,Es,It,Nl) (GB Comp.)",
        "DMG-ABLP",
        4039917695,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "a73dea6a3f16fb70ebd7a8bd4625c1874559a84c",
        "Bakuchou Retrieve Master",
        "(Japan) (SGB)",
        "DMG-ABMJ",
        2320788972,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "dc6c8141aef77414dbafb9907155de5d1153e879",
        "Burger Burger Pocket - Hamburger Simulation",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-ABNJ",
        448589246,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "5b610b624de0a7f9fbbbfd05f117fe4f23cb323e",
        "Super Breakout",
        "(USA)",
        "DMG-ABOE",
        499185683,
        131072,
        "En",
        "USA"
   , " " },
    {
        "1a07ea3f2f2490797186729acd0932f5ec547699",
        "Super Black Bass Pocket",
        "(Japan) (SGB)",
        "DMG-ABPJ",
        2323388024,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "2c9570016a2debde59c63b5dc933817eb1491871",
        "Super Black Bass Pocket 3",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-ABQJ",
        2923849029,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "9a0fb3744b76e8ec818b587dd8fec484a56ef54e",
        "Brain Drain",
        "(USA) (SGB)",
        "DMG-ABRE",
        2951786841,
        131072,
        "En",
        "USA"
   , " " },
    {
        "114138976f5eba947748ab1c858b14ad93c0c505",
        "Brain Drain",
        "(Japan) (En) (SGB)",
        "DMG-ABRJ",
        3847793357,
        131072,
        "En",
        "Japan"
   , " " },
    {
        "525290fa07ff8f3656071a663e4164438191ed15",
        "Brain Drain",
        "(Europe) (SGB)",
        "DMG-ABRP",
        2323624166,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "a93313bbbeb97da5a5736a258e23c1373a8f1f0d",
        "Arcade Classics - Super Breakout & Battlezone",
        "(USA, Europe) (SGB)",
        "DMG-ABSE",
        1814636345,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "03eed25bab9085ddae979c4e8fd2aea5c63c199c",
        "Super Black Bass Pocket 2",
        "(Japan) (SGB)",
        "DMG-ABTJ",
        1755665334,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "e8e07a7be651e32d6ba47a24cf8814b1be805cd0",
        "Bust-A-Move 2 - Arcade Edition",
        "(USA, Europe)",
        "DMG-ABUE",
        3108447462,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "9e9d47c8ef9c90180ee157c4ba9656d40f9d6e8f",
        "Beethoven",
        "(Europe) (SGB)",
        "DMG-ABVP",
        1669223651,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "4b64de6053c42de0a112213caeafaac2aa3f1ce3",
        "BreakThru!",
        "(USA)",
        "DMG-ABXE",
        1536101874,
        131072,
        "En",
        "USA"
   , " " },
    {
        "488a08e7b0504ae939ca6353dec21ee81ade4639",
        "NFL Blitz",
        "(USA, Europe) (Rev 1) (GB Comp.)",
        "DMG-ABZE",
        276656971,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "b5ecb06aac1502d0c2a2c8ec78ee090df8e82216",
        "NFL Blitz",
        "(USA, Europe) (GB Comp.)",
        "DMG-ABZE",
        3610369751,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "4cb0e20a1244c3d74abd066ae9a7a52571608a44",
        "Goukaku Boy Series - Z-Kai Kyuukyoku no Eitango 1500",
        "(Japan)",
        "DMG-AC2J",
        1068260418,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "317cc7f3e7af5e3062fdd86ef686258204815076",
        "Choplifter III",
        "(Europe)",
        "DMG-AC3E",
        456869615,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "789ade333c9cd442e30c6894da252ef0c32cdf7d",
        "Centipede",
        "(USA) (Majesco)",
        "DMG-AC4E",
        3914793295,
        131072,
        "En",
        "USA"
   , " " },
    {
        "4645cfcca1cb31bd12ba6b933702c3b03e1cd238",
        "Centipede",
        "(USA) (GB Comp.)",
        "DMG-AC5E",
        330106801,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "82d73d6983299b3fabece78ecaa217a523c9b0fd",
        "Centipede",
        "(Europe) (En,Fr,De,Es,It,Nl) (GB Comp.)",
        "DMG-AC5P",
        626899134,
        524288,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "2b78e9f70eb88f616eb5e710f0dc0f3c62250675",
        "Pocket Color Block",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AC6J",
        949810543,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "5c1eee613486fb6f174a0883d86a374fb1a21514",
        "Tokimeki Memorial Pocket - Culture Hen - Komorebi no Melody",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AC7J",
        1273276653,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "17715b7dd8a2ffa03f22e637eb7229cd0b54ee2a",
        "CutThroat Island",
        "(USA, Europe)",
        "DMG-AC8E",
        4005417824,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "50681e168af33cd6ce18e36cd98f44bd2f5844bb",
        "Chessmaster",
        "(USA, Europe) (GB Comp.)",
        "DMG-AC9E",
        471063472,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "496ea9f7d73bc902c325645d28fb89ceeda970e7",
        "Nintama Rantarou GB - Eawase Challenge Puzzle",
        "(Japan) (SGB)",
        "DMG-ACAJ",
        238543970,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "5-37922",
        "Akazukin Chacha",
        "(Japan) (SGB)",
        "DMG-ACCJ",
        2307262900,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "a539e667b8fde8593a82a0d7b27bd0f5f8cee425",
        "Cool Hand",
        "(Europe)",
        "DMG-ACDP",
        222498074,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "37217dbbdefcf965c27d72d26a0574b69323ba04",
        "Cool Hand",
        "(Europe) (Fr,De)",
        "DMG-ACDX",
        2733643413,
        262144,
        "Fr,De",
        "Europe"
   , " " },
    {
        "fb2f3b8241640c94c06d7659b78a83c2be5c0b56",
        "NBA All-Star Challenge",
        "(USA, Europe)",
        "DMG-ACE",
        4066093247,
        65536,
        "En",
        "USA, Europe"
   , " " },
    {
        "34434ed90f8f42f6b3235663e7f307c29cccf7b8",
        "Casper",
        "(USA)",
        "DMG-ACEE",
        3866759393,
        131072,
        "En",
        "USA"
   , " " },
    {
        "129a1672f423995227509fb57b39e59cbf33547d",
        "Casper",
        "(Europe)",
        "DMG-ACEP",
        3836937780,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "d3ee2fc326aab45075c2104dac364bf6e16b8cf1",
        "Casino FunPak",
        "(USA, Europe)",
        "DMG-ACFE",
        2252454485,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "102-16232",
        "Pinocchio",
        "(USA)",
        "DMG-ACGE",
        764563014,
        262144,
        "En",
        "USA"
   , " " },
    {
        "c71b0c2ad947c1add7b1d192c8b3e39a024e3b74",
        "Pinocchio",
        "(Europe)",
        "DMG-ACGP",
        140265986,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "2056f73c38c59e79d0c623a6c86c670a89292128",
        "New Chessmaster, The",
        "(Japan) (En,Ja)",
        "DMG-ACHJ",
        1578909325,
        65536,
        "En,Ja",
        "Japan"
   , " " },
    {
        "3bfef4f241d4c12be10670ce6f0eace5295dc991",
        "Jinsei Game - Tomodachi Takusan Tsukurou yo!",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-ACJJ",
        3369365145,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "1f4ad88ee5897e9d102184e715fb0b26cd411079",
        "Go Go Ackman",
        "(Japan) (SGB)",
        "DMG-ACKJ",
        1641548102,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "aff8b5d008e6173ce374b3c8e8a9ef99a8f934c9",
        "Super Chinese Land 3",
        "(Japan) (SGB)",
        "DMG-ACLJ",
        3759976203,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "241b4baf2596eb2800331c66947e14e6d4a5937a",
        "Chibi Maruko-chan - Maruko Deluxe Theater",
        "(Japan) (SGB)",
        "DMG-ACMJ",
        2288011769,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "ffd936b1c81d37f5685ad3e69f290e2a0cc44a20",
        "Quest for Camelot",
        "(USA, Australia) (En,Fr,Es) (SGB) (GB Comp.)",
        "DMG-ACNE",
        3640908669,
        1048576,
        "En,Fr,Es",
        "USA, Australia"
   , " " },
    {
        "d3e01914b4224d87289d1c23e9ebda7fdc87053b",
        "Quest for Camelot",
        "(Europe) (En,Fr,De,Es,It,Nl) (SGB) (GB Comp.)",
        "DMG-ACNP",
        3461719262,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "ecb1dd837785f75e7e60018d4b468ddbc18d0b77",
        "Collection Pocket",
        "(Japan) (SGB)",
        "DMG-ACOJ",
        887499915,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "ea77766523a08f5159f5207a5c0b5f474a50b182",
        "Arcade Classic No. 2 - Centipede & Millipede",
        "(USA, Europe) (SGB)",
        "DMG-ACPE",
        2974563121,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "5df158452f82569a9578b51ca6e2ee01e19f7671",
        "Choro Q - Hyper Customable GB",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-ACQJ",
        1700859044,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "8b8bc89e8e9084a7165f2c66ce8e02db8c575a49",
        "Conker's Pocket Tales",
        "(USA, Europe) (En,Fr,De) (SGB) (GB Comp.)",
        "DMG-ACRE",
        2769021352,
        2097152,
        "En,Fr,De",
        "USA, Europe"
   , " " },
    {
        "8a04c460e4a8efe4ca486d04e5e6641705b8c13e",
        "Contra - The Alien Wars",
        "(USA) (SGB)",
        "DMG-ACSE",
        4056424112,
        131072,
        "En",
        "USA"
   , " " },
    {
        "f2dc341340327963dd20153840a7892ad9e02c24",
        "Contra Spirits",
        "(Japan) (SGB)",
        "DMG-ACSJ",
        2143432181,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "5ad9d88e031d562e0b0fe2c790a963771d041eeb",
        "Probotector 2",
        "(Europe) (SGB)",
        "DMG-ACSP",
        3037269571,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "d7f61aee11eca10c7a539b319d93d157c8aed701",
        "Captain Tsubasa J - Zenkoku Seiha e no Chousen",
        "(Japan) (SGB)",
        "DMG-ACTJ",
        866247119,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "88e154df6cb144e42acc91bc784899daf4a8cd6d",
        "Puchi Carat",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-ACUJ",
        4040213357,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "589bf4e15d39b2e9a6e56cd2792ba3679faf89ce",
        "NBA 3 on 3 Featuring Kobe Bryant",
        "(USA) (SGB) (GB Comp.)",
        "DMG-ACVE",
        2049876,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "704a4a56425b9463400726e17d47ebc437f65633",
        "Las Vegas Cool Hand",
        "(USA) (GB Comp.)",
        "DMG-ACWE",
        3203517924,
        524288,
        "En",
        "USA"
   , " " },
    {
        "8d050fd7f813a439d1dfa957aef3506cc599edbf",
        "Cool Hand",
        "(Europe) (En,Fr,De) (GB Comp.)",
        "DMG-ACWP",
        3871940536,
        524288,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "205-54566",
        "Pokemon Card GB",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-ACXJ",
        421983600,
        1048576,
        "Ja",
        "Japan",
        "/pokemon_tcg64.sprite" 
    },
    {
        "cc0a7cd04776c4a0a39e22a21aecef3cfae7ccaa",
        "Checkmate",
        "(Japan) (En,Ja) (GB Comp.)",
        "DMG-ACZJ",
        447431005,
        1048576,
        "En,Ja",
        "Japan"
   , " " },
    {
        "d72cbe6b28b93466d8ee94ba21e93fcfac58fc3b",
        "Goukaku Boy Series - Koukou Nyuushi Derujun - Chuugaku Eijukugo 350",
        "(Japan) (Imagineer)",
        "DMG-AD2J",
        3827769163,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "3cd40f5268854d3cc4d3937c5d0dcca8b59148e6",
        "Donkey Kong Land III",
        "(USA, Europe) (Rev 1) (SGB)",
        "DMG-AD3E",
        2711276982,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "ba6e1031d8fb1415bb7898ca19eebdd31d065437",
        "Donkey Kong Land III",
        "(USA, Europe) (SGB)",
        "DMG-AD3E",
        3020690844,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "5269be0e6eeddbe927fb6ad0d350255843d3cc7a",
        "Donkey Kong GB - Dinky Kong & Dixie Kong",
        "(Japan)",
        "CGB-AD3J",
        685238483,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "55ee5d7b36e548a592ecf754692f13ebff103da9",
        "Dropzone",
        "(Europe) (GB Comp.)",
        "DMG-AD4P",
        510768766,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "e679401aa2ad407e3c557c76fa00705c630c372e",
        "Daffy Duck - Fowl Play",
        "(USA, Europe) (GB Comp.)",
        "DMG-AD7E",
        1173090460,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "74cd407f8922f6483fbe78e2df7f484e3c1e8a43",
        "Momotarou Dentetsu jr. - Zenkoku Ramen Meguri no Maki",
        "(Japan) (SGB)",
        "DMG-AD8J",
        562193843,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "07f4bd664b37b716281e93250630313da0d79ff6",
        "Druaga no Tou",
        "(Japan)",
        "DMG-ADA",
        4179934641,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "eda26b2c57661d6a5b65486f7925e12683aa33cd",
        "Doraemon - Aruke Aruke Labyrinth",
        "(Japan) (GB Comp.)",
        "DMG-ADAJ",
        4141324921,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "f489acabde4689237fe3e9b79120bd7be260134e",
        "Dragon Ball Z - Gokuu Hishouden",
        "(Japan) (SGB)",
        "DMG-ADBJ",
        953829079,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "4f1eebda83514d2cc51a8c6d8c13fad9cecc62dd",
        "Meitantei Conan - Chika Yuuenchi Satsujin Jiken",
        "(Japan) (SGB)",
        "DMG-ADCJ",
        960871437,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "b524e9529d697b3b2833cb1f81247589ba16ebf8",
        "Donkey Kong Land 2",
        "(USA, Europe) (SGB)",
        "DMG-ADDE",
        673703380,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "272dc5c9560f368ccc9ca27a2010982f7a6ac009",
        "Donkey Kong Land",
        "(Japan) (SGB)",
        "DMG-ADDJ",
        2599198812,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "79dae4414184c1b3972b2f1f5116986ebfc55a75",
        "Tokyo Disneyland - Fantasy Tour",
        "(Japan) (SGB)",
        "DMG-ADFJ",
        683169156,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "b55f963dea904f4fbb18e6a446f8641e20b8a8dc",
        "DragonHeart",
        "(USA, Europe)",
        "DMG-ADHE",
        1361683736,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "b59263d9da9fb96379eeb2be82e0d617e37b9b9a",
        "DragonHeart",
        "(France)",
        "DMG-ADHF",
        2366149462,
        524288,
        "Fr",
        "France"
   , " " },
    {
        "396eaed2379fccc2228e10252c42ae759ec4dccf",
        "Duke Nukem",
        "(USA) (En,Fr,De,Es,It)",
        "CGB-ADIE",
        2221918256,
        1048576,
        "En,Fr,De,Es,It",
        "USA"
   , " " },
    {
        "7047a8094b65c8ce89d7a409e9b9674312f377c6",
        "Duke Nukem",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-ADIP",
        4231863223,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "1dcd8a70f33f01911669ca1826f34d8ca332cc47",
        "Arcade Classic No. 4 - Defender & Joust",
        "(USA, Europe) (SGB)",
        "DMG-ADJE",
        478321891,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "20b3e80e2b4832d2b3a2e5ab2bc3ba9473a5618e",
        "Meitantei Conan - Giwaku no Gouka Ressha",
        "(Japan) (SGB)",
        "DMG-ADKJ",
        499499802,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "1109d4d893f6726bd06338918fa3d9935f343eba",
        "Tokyo Disneyland - Mickey no Cinderella-jou Mystery Tour",
        "(Japan) (SGB)",
        "DMG-ADLJ",
        2010551113,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "3e777585ab8738d1f3c5e3da7b528c39858c36c6",
        "Game Boy Color",
        "(World) (Demo) (Kiosk)",
        "CGB-ADME",
        4125731468,
        524288,
        "En",
        "World"
   , " " },
    {
        "2e31e1499245c92547ec4e74e0b31281b836c62d",
        "Maus, Die",
        "(Europe) (En,Fr,De,Es)",
        "DMG-ADMP",
        1200576316,
        262144,
        "En,Fr,De,Es",
        "Europe"
   , " " },
    {
        "dab3f34bdf0ab1957e8b944b60ea9706a21c0eb9",
        "Castlevania Legends",
        "(USA, Europe) (SGB)",
        "DMG-ADNE",
        2912688123,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "5-13399",
        "Akumajou Dracula - Shikkoku Taru Zensoukyoku - Dark Night Prelude",
        "(Japan) (SGB)",
        "DMG-ADNJ",
        1210430047,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "af194c38ef22ae21e7ac3214c2513194b5e6a31c",
        "Doraemon Kart 2",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-ADOJ",
        3474595896,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "13fa9d075f364445e01e114079d34fbaa2653d5e",
        "Doraemon no Game Boy de Asobouyo Deluxe 10",
        "(Japan) (SGB)",
        "DMG-ADPJ",
        3009996567,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "83f6d3ad92a4ec0c251e4e4602b13778acd633a5",
        "Dragon Quest Monsters - Terry no Wonderland",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-ADQJ",
        1191363825,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "879145e714adb86d299739d11fb017e1307b52d3",
        "Dragon Quest Monsters - Terry no Wonderland",
        "(Japan) (Rev 1) (SGB) (GB Comp.)",
        "DMG-ADQJ",
        3529758861,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "5207ccde86620e0d669dc2821dba420fc5d935fb",
        "Doraemon Kart",
        "(Japan) (SGB)",
        "DMG-ADRJ",
        3712899215,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "5cfebe0ef12affaf6509b9e0e68e8b749b75ed46",
        "Desert Strike - Return to the Gulf",
        "(USA) (SGB)",
        "DMG-ADSE",
        1785736242,
        262144,
        "En",
        "USA"
   , " " },
    {
        "736cf97bf66f3c363b63d4aefc7f5174a4c5adc5",
        "Desert Strike - Return to the Gulf",
        "(Europe) (SGB)",
        "DMG-ADSP",
        3070294007,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "2e9cf72465dbc5028d1c1e66a4a9e791f3badeca",
        "Deja Vu I & II - The Casebooks of Ace Harding",
        "(USA)",
        "CGB-ADTE",
        2903062516,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "1574d72b80d2ad7cd1fd4d8d3e001df68d45ab9c",
        "Deja Vu I & II",
        "(Japan)",
        "CGB-ADTJ",
        3136474262,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "dfd7723979a5b367225396c2dd5b98491a7f9b1c",
        "Deja Vu I & II - The Casebooks of Ace Harding",
        "(Europe) (En,Fr)",
        "CGB-ADTX",
        1912051893,
        1048576,
        "En,Fr",
        "Europe"
   , " " },
    {
        "bf1caaea20db008e14c4aca5c2f7b324116a5e62",
        "Deja Vu I & II - The Casebooks of Ace Harding",
        "(Europe) (Fr,De)",
        "CGB-ADTY",
        3337089990,
        1048576,
        "Fr,De",
        "Europe"
   , " " },
    {
        "cc8bdf4a87744b342aa3c4a77501e979111e2071",
        "Midway Presents Arcade Hits - Moon Patrol & Spy Hunter",
        "(USA, Europe) (GB Comp.)",
        "DMG-ADUE",
        4093745609,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "85aac8c89dc60a8a661d58fed34ca90e53bc52a0",
        "Granduel - Shinki Dungeon no Hihou",
        "(Japan)",
        "CGB-ADVJ",
        3582575873,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "c2c07ced1ec5a61ba6ab6d5cf5f0ee42a8d7896e",
        "Dungeon Savior",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-ADWJ",
        734748536,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "19b9b2229b5fdf2cdfbbd0ec8d0abf13be27271e",
        "Maus, Die",
        "(Europe) (En,Fr,De,Es)",
        "CGB-ADXP",
        3887942217,
        1048576,
        "En,Fr,De,Es",
        "Europe"
   , " " },
    {
        "ae2d00d737e5f623ff616b236957cfdb3c281b4d",
        "Barbie - Meeres Abenteuer",
        "(Germany) (GB Comp.)",
        "DMG-ADYD",
        1581700682,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "f91deaf99632cd585709f117bcc0446fc2268ffd",
        "Barbie - Ocean Discovery",
        "(USA) (GB Comp.)",
        "DMG-ADYE",
        1953052358,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "485b73967efc24d0b721aefe7199195957c3eed0",
        "Barbie - Chasse au Tresor Sous-Marine",
        "(France) (GB Comp.)",
        "DMG-ADYF",
        1080617345,
        1048576,
        "Fr",
        "France"
   , " " },
    {
        "18bead2760722750e9b84ecdd6ebd404d4f76167",
        "Barbie - Diepzee Avontuur",
        "(Netherlands) (GB Comp.)",
        "DMG-ADYH",
        969324767,
        1048576,
        "Nl",
        "Netherlands"
   , " " },
    {
        "685219616a83476e07d857c405db0ae5e92cf5f4",
        "Barbie - Avventure nell'Oceano",
        "(Italy) (GB Comp.)",
        "DMG-ADYI",
        1499748888,
        1048576,
        "It",
        "Italy"
   , " " },
    {
        "e156267f78b3eb22976df99bcbc2136aeb3954c2",
        "Barbie - Ocean Discovery",
        "(United Kingdom) (GB Comp.)",
        "DMG-ADYP",
        4257802225,
        1048576,
        "En",
        "United Kingdom"
   , " " },
    {
        "6dec77093fcc1e138ecd95459997aed24ad20795",
        "Barbie - Aventura Submarina",
        "(Spain) (GB Comp.)",
        "DMG-ADYS",
        217233394,
        1048576,
        "Es",
        "Spain"
   , " " },
    {
        "d5fae48f4d002feac067bd4708cd927d3bcf27bd",
        "Daikaijuu Monogatari - Poyon no Dungeon Room",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-ADZJ",
        792104358,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "04b4fed727d905a85f455ce5c5e0cad153f0a7be",
        "Goukaku Boy Series - Eiken 2kyuu Level no Kaiwa Hyougen 333",
        "(Japan)",
        "DMG-AE2J",
        1185911002,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "4869724ad109ab262e60002c927d2d2aad96397a",
        "Sesame Street - Elmo's 123s",
        "(USA) (GB Comp.)",
        "DMG-AE3E",
        406059832,
        262144,
        "En",
        "USA"
   , " " },
    {
        "d33c68b02c66653e8e08dc38493131cbe7a5b2f4",
        "Sesame Street - Elmo's 123s",
        "(Europe) (GB Comp.)",
        "DMG-AE3P",
        1006107860,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "41a1b008442e7a7746dec0a9173ec75bacd90313",
        "Goukaku Boy Series - Shikakui Atama o Maruku Suru - Rika Battle Hen",
        "(Japan) (Imagineer)",
        "DMG-AE8J",
        3276465039,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "247-59509",
        "Aerostar",
        "(Japan)",
        "DMG-AEA",
        3752047242,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "3078da312eecb6ff32b5880b436a85a7fb0e0fcb",
        "Sesame Street - Elmo's ABCs",
        "(USA) (GB Comp.)",
        "DMG-AEAE",
        3424629417,
        262144,
        "En",
        "USA"
   , " " },
    {
        "cb64a50a70c73285a8e75cbec68df276834ccecc",
        "Sesame Street - Elmo's ABCs",
        "(Europe) (GB Comp.)",
        "DMG-AEAP",
        538283964,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "c6d9b605a3f732d4a806ea176a82c7812c12e051",
        "Earthworm Jim - Menace 2 the Galaxy",
        "(USA, Europe) (GB Comp.)",
        "DMG-AEBE",
        778427055,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "2176abdd77a7424be8d29d911c467b9205033ded",
        "Real Pro Yakyuu! - Central League Hen",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AECJ",
        2952752057,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "994b3612269d31545465f8639cf2b12b7c8b779c",
        "Dragon Warrior I & II",
        "(USA) (SGB) (GB Comp.)",
        "DMG-AEDE",
        1909887962,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "147373e900b027162baae3641002209ee9267ebd",
        "Dragon Quest I & II",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AEDJ",
        2689840174,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "246-45151",
        "Aerostar",
        "(USA, Europe)",
        "DMG-AEE",
        4143785822,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "cdba8fde15c6f66c9946878e27297022ad30ab24",
        "Ganbare Goemon - Mononoke Douchuu Tobidase Nabebugyou!",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AEFJ",
        1932598522,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "498baec72535178354c938fcb230c0c06b192542",
        "Blaster Master - Enemy Below",
        "(USA, Europe) (SGB) (GB Comp.)",
        "DMG-AEHE",
        798089596,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "8cf8acbe7cfd674747d48ac9e8796d691cf805f4",
        "Kawaii Pet Shop Monogatari",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AEIJ",
        1148613699,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "977f11f5fecdfdbdce4a63244be69feb9328edb8",
        "Earthworm Jim",
        "(USA)",
        "DMG-AEJE",
        631239271,
        262144,
        "En",
        "USA"
   , " " },
    {
        "a027942e4ee6dcf543fbea7436b91db349f68f41",
        "Earthworm Jim",
        "(Europe)",
        "DMG-AEJP",
        2980552712,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "ac6a723c9e83157d15bd097f4a5ff1b68afb2754",
        "Sesame Street - The Adventures of Elmo in Grouchland",
        "(USA) (GB Comp.)",
        "DMG-AELE",
        743189087,
        262144,
        "En",
        "USA"
   , " " },
    {
        "11729f02793c090da14b50a635716c00bcc89468",
        "Sesame Street - The Adventures of Elmo in Grouchland",
        "(Europe) (GB Comp.)",
        "DMG-AELP",
        1092783847,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "67df4bad52e2da41c85f1134f9172393c7aa7aa2",
        "Madden NFL 2000",
        "(USA, Europe) (SGB) (GB Comp.)",
        "DMG-AEME",
        1210664106,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "457dda83a338cd45137deda0a3ed58c86cf61202",
        "Sei Hai Densetsu",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AEOJ",
        2282027094,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "e4366b272c387aca32b18697ab8ce48074d0da7b",
        "Real Pro Yakyuu! - Pacific League Hen",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AEPJ",
        381164598,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "24e224aa3d03e379f45e0af791a4b5235b991b62",
        "F1 Racing Championship",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-AEQP",
        2477357215,
        4194304,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "7057bc68d297eaa39334975854caec9ef9610535",
        "J.League Excite Stage GB",
        "(Japan)",
        "CGB-AESJ",
        3019083542,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "82a9e7e0b9ca72ecca57c5bd4dde200efaba40da",
        "Evel Knievel",
        "(USA) (GB Comp.)",
        "DMG-AEVE",
        1374245301,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "597c2aae1a2487b67edde4e6f6772d061624dff0",
        "Evel Knievel",
        "(Europe) (En,Fr,De,Es,It,Nl,Sv) (GB Comp.)",
        "DMG-AEVP",
        3755387144,
        2097152,
        "En,Fr,De,Es,It,Nl,Sv",
        "Europe"
   , " " },
    {
        "04a1bdb1e83682195f12480945babe1f4dc9574d",
        "Wizardry Empire",
        "(Japan)",
        "CGB-AEWJ",
        2061275361,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "2e60690a8753e5749ed8ff3adc1684aac7b6bd32",
        "Wizardry Empire",
        "(Japan) (Rev 1)",
        "CGB-AEWJ",
        4202848783,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "cf5ad603209db045ffb42537e1f20d442258c1d9",
        "Gex - Enter the Gecko",
        "(USA, Europe) (GB Comp.)",
        "DMG-AEXE",
        3704386836,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "f472d8b1a0d9f437ad8037d116a685ce6fe27cf0",
        "Burger Paradise International",
        "(Japan)",
        "CGB-AEYJ",
        2425532651,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "25435c6a0380b49686c5ae09e3e18076e484127e",
        "Tsuri Sensei 2",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AF2J",
        467667566,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "f55f91dd92c6d4a8ca562bbf8bb11eaa77d49492",
        "Goukaku Boy Series - Shikakui Atama o Maruku Suru - Sansuu Battle Hen",
        "(Japan) (Imagineer)",
        "DMG-AF3J",
        229488681,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "e017a3e26bd87908a8b1c20158592999eb04814a",
        "Hello Kitty's Cube Frenzy",
        "(USA) (GB Comp.)",
        "DMG-AF4E",
        2474060278,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "303083c507e54e590b35e0f2b5a0ebcc296a4311",
        "Konami GB Collection Vol.1",
        "(Europe) (GB Comp.)",
        "DMG-AF5P",
        541034279,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "32e6a825c99726257039c5ebfbc0c4272774e7de",
        "Konami GB Collection Vol.2",
        "(Europe) (GB Comp.)",
        "DMG-AF6P",
        2789840786,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "d5bc9e93daac291fc0774b649fd797d7b8c9d203",
        "Konami GB Collection Vol.3",
        "(Europe) (GB Comp.)",
        "DMG-AF7P",
        3570803773,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "e0d7344885187a5e0fefd9b409961b655c638818",
        "Konami GB Collection Vol.4",
        "(Europe) (GB Comp.)",
        "DMG-AF8P",
        2281763273,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "e1bd8c8a89868afc0514105f62894d73033c416b",
        "Casper",
        "(USA)",
        "CGB-AF9E",
        3346388563,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "f8e22f28a8ff7ef1155761dccfd7deb58bf1db1c",
        "Casper",
        "(Europe) (En,Fr,De)",
        "CGB-AF9P",
        3870945621,
        1048576,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "ba0e65ecd69e99bfb71a09c5ab592e2f14574002",
        "Casper",
        "(Europe) (En,Es,It)",
        "CGB-AF9X",
        3673154876,
        1048576,
        "En,Es,It",
        "Europe"
   , " " },
    {
        "6b6875f4da694c82eefc43d49812421b8716529a",
        "TNN Outdoors Fishing Champ",
        "(USA) (SGB) (GB Comp.)",
        "DMG-AFCE",
        10571032,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "b3304c91ad0cd23c335ee0a2006dc43a52d5a6a2",
        "Fushigi no Dungeon - Fuurai no Shiren GB - Tsukikage Mura no Kaibutsu",
        "(Japan) (SGB)",
        "DMG-AFDJ",
        694333364,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "240-1109",
        "Addams Family, The",
        "(USA)",
        "DMG-AFE",
        3245385522,
        131072,
        "En",
        "USA"
   , " " },
    {
        "5c5d9e3cc3d3866e920f3faa21dafd5b64679b4c",
        "Foreman for Real",
        "(USA, Europe)",
        "DMG-AFEE",
        1997028064,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "678b1455c10aceec5d3a7c5029da385eb6057e8c",
        "Foreman for Real",
        "(Japan)",
        "DMG-AFEJ",
        2249107478,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "095aa1bbbfd8fab381429732978f1f4fee6283bf",
        "Super Chinese Fighter GB",
        "(Japan) (SGB)",
        "DMG-AFFJ",
        3039207702,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "fbd73dc7fbead1aefa5e3457a3091d5145f42580",
        "Frogger",
        "(USA)",
        "DMG-AFGE",
        1301494792,
        131072,
        "En",
        "USA"
   , " " },
    {
        "5b3c24ecb06efa8c6c58717741fc1662523747c0",
        "Legend of the River King GB",
        "(USA) (SGB)",
        "DMG-AFHE",
        2800125404,
        524288,
        "En",
        "USA"
   , " " },
    {
        "b7ad2a0521d60680cd05e60f9f40d34700a9a4a5",
        "Legend of the River King GB",
        "(Australia) (SGB)",
        "DMG-AFHU",
        1365406227,
        524288,
        "En",
        "Australia"
   , " " },
    {
        "b4cc278944ed51246afa038617b4b2d08bbde0d7",
        "F-1 World Grand Prix",
        "(Europe) (En,Fr,De,Es)",
        "CGB-AFIE",
        2375717250,
        2097152,
        "En,Fr,De,Es",
        "Europe"
   , " " },
    {
        "216-11073",
        "Addams Family, The",
        "(Japan)",
        "DMG-AFJ",
        1282710548,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "bb515c5d7f54ed5887f15e73c6fac9483e5c9307",
        "Snoopy no Hajimete no Otsukai",
        "(Japan) (SGB)",
        "DMG-AFJJ",
        2298521536,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "dfc6a243f20a4567e95d50eb5f2d7c33f43b1f84",
        "Frank Thomas Big Hurt Baseball",
        "(USA, Europe)",
        "DMG-AFKE",
        2588494566,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "a2a8a356d5bb537aa8791a2fb4e7c223c2890f25",
        "Pitfall - Beyond the Jungle",
        "(USA, Europe) (GB Comp.)",
        "DMG-AFLE",
        4178688861,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "ae04e811188621fb603da517d1b80c669c3078a5",
        "Pitfall GB",
        "(Japan) (GB Comp.)",
        "DMG-AFLJ",
        4194946750,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "d89ab7cb1965075c07c73bdd323edf9e192420cb",
        "Fushigi no Dungeon - Fuurai no Shiren GB 2 - Sabaku no Majou",
        "(Japan) (AFMJ)",
        "CGB-AFMJ",
        748153103,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "1aeb4bafbc3460c8c3f2af2d9b356eb8f8ae9675",
        "Flintstones, The",
        "(USA, Europe)",
        "DMG-AFNE",
        1346188819,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "fde669fab4a51e4b28c77c246a2852f9844faeea",
        "FIFA 2000",
        "(USA, Europe) (SGB) (GB Comp.)",
        "DMG-AFOE",
        3871092364,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "2b6674d412ab628205394cbc6ad78bb0576ced76",
        "Frogger",
        "(USA) (GB Comp.)",
        "DMG-AFRE",
        2940660343,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "653620c6477f844a06e562e9b569b1373236b87f",
        "Frogger",
        "(USA) (Rev 1) (GB Comp.)",
        "DMG-AFRE",
        3130028132,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "67bb53269dc2d71ce8c3b50e55d96e33c03fa451",
        "Frogger",
        "(USA) (Rev 2) (GB Comp.)",
        "DMG-AFRE",
        3651284573,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "3d89f6484223d506b89ec3f6d043518ddcddcf01",
        "Frogger",
        "(Europe) (En,Fr,De,Es,It,Nl) (GB Comp.)",
        "DMG-AFRP",
        3065972338,
        524288,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "38fc4f98cc9cbfaf3b75c2646d85c20620a6ad7b",
        "FIFA International Soccer",
        "(USA, Europe) (En,Fr,De,Es) (SGB)",
        "DMG-AFSE",
        3851983112,
        524288,
        "En,Fr,De,Es",
        "USA, Europe"
   , " " },
    {
        "36ed2a7e958b32cd796a8ce005792b8a80b79b3f",
        "Frisky Tom",
        "(Japan) (SGB)",
        "DMG-AFTJ",
        200246433,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "627ea6d514f9f65cc940315e29fd32c764cc7c15",
        "Fairy Kitty no Kaiun Jiten - Yousei no Kuni no Uranai Shugyou",
        "(Japan) (Rev 1) (SGB) (GB Comp.)",
        "DMG-AFUJ",
        3302187663,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "abb773d14628a5ced095c469a02fcd6acdb717a9",
        "Fairy Kitty no Kaiun Jiten - Yousei no Kuni no Uranai Shugyou",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AFUJ",
        3046789572,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "5f5a3918a2ffc09cb163b8b855f6b87f3432f601",
        "Tsuri Sensei",
        "(Japan) (SGB)",
        "DMG-AFWJ",
        3694280960,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "240-57662",
        "Addams Family, The",
        "(Europe) (En,Fr,De)",
        "DMG-AFX",
        687490334,
        131072,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "cb3f8ea1ced708ce438ad1a27c7f2f410a0aa85b",
        "Wings of Fury",
        "(USA) (GB Comp.)",
        "DMG-AFXE",
        3377086778,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "001a3483df815d6c1041e7600fd8696df43c177a",
        "Wings of Fury",
        "(Europe) (En,Fr,De) (GB Comp.)",
        "DMG-AFXP",
        1834553423,
        1048576,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "2cee60aebb881283e272300365ba888e98d5d237",
        "Street Fighter Alpha - Warriors' Dreams",
        "(USA)",
        "CGB-AFZE",
        2858357970,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "fcda05cbb15b98a01d1a8d38888521bbe5793231",
        "Street Fighter Alpha - Warriors' Dreams",
        "(Europe)",
        "CGB-AFZP",
        681814842,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "8af7fdda1e5886fbd01d82a9ab41eec03134d029",
        "Dai-2-ji Super Robot Taisen G",
        "(Japan) (SGB)",
        "DMG-AG2J",
        3750439712,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "3e7c482eb01e4c484f1aee778ba6dae70a64e4e3",
        "Genjin Collection",
        "(Japan) (SGB)",
        "DMG-AG3J",
        3001998646,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "3737f3613f48b1f315027f17209b9eb936ae81da",
        "Goukaku Boy Series - Kirihara Shoten Hinshutsu Eibunpou Gohou Mondai 1000",
        "(Japan)",
        "DMG-AG4J",
        1103197217,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "16ac1f2b9b6779a9d835d027dfb6d688352e2f5c",
        "Jisedai Begoma Battle Beyblade",
        "(Japan)",
        "CGB-AG5J",
        2622920574,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "a16763feaa54a251c442b5b63c14f9066e0a5872",
        "Goukaku Boy Series - Koukou Nyuushi Derujun - Kanji Mondai no Seifuku",
        "(Japan)",
        "DMG-AG6J",
        2229472876,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "6d3bb7ddd742d96cb3ee35cb9531d8c8730426d7",
        "Sanrio Uranai Party",
        "(Japan) (Rev 1)",
        "DMG-AG7J",
        3496508831,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "837043ed8b57f710ddaeac0ae57a28b6e723b9b9",
        "Sanrio Uranai Party",
        "(Japan)",
        "DMG-AG7J",
        223085223,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "63fc42e49c89d2d73dc42167401c1f951b904772",
        "Ghosts'n Goblins",
        "(USA, Europe) (GB Comp.)",
        "DMG-AG9E",
        2919386147,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "5a7fa3ed959f3299f7108f13c3212973a6c7afc8",
        "Game & Watch Gallery",
        "(USA) (Rev 1) (SGB)",
        "DMG-AGAE",
        2657934486,
        262144,
        "En",
        "USA"
   , " " },
    {
        "b27f24117b30e88423a8d3f993fd1d055538d72a",
        "Game & Watch Gallery",
        "(USA) (SGB)",
        "DMG-AGAE",
        2578644601,
        262144,
        "En",
        "USA"
   , " " },
    {
        "dd4cdb1767387a4b702022a57421a002aad58d53",
        "Game Boy Gallery",
        "(Japan) (SGB)",
        "DMG-AGAJ",
        2839417435,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "5c0ca40b8f9fa25b91659942f908956a68a09691",
        "Game & Watch Gallery",
        "(Europe) (SGB)",
        "DMG-AGAP",
        2532098528,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "7e4349f3eec985c5ef6e2719858e8343a3bbd2da",
        "Game Boy Gallery 2",
        "(Australia) (SGB)",
        "DMG-AGAU",
        1795772559,
        262144,
        "En",
        "Australia"
   , " " },
    {
        "3152ca804a088ba553d0404c93844510c6f31ec3",
        "Kuusou Kagaku Sekai Gulliver Boy - Kuusou Kagaku Puzzle Purittopon!!",
        "(Japan) (SGB)",
        "DMG-AGBJ",
        4213775992,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "49417e6b7d76d068727f68e477f87f7c130f20f4",
        "Arcade Classic No. 3 - Galaga & Galaxian",
        "(USA) (SGB)",
        "DMG-AGCE",
        1785647084,
        131072,
        "En",
        "USA"
   , " " },
    {
        "1d1cf6a6680737d7b8e5109235b6f56b70acd015",
        "Galaga & Galaxian",
        "(Japan) (SGB)",
        "DMG-AGCJ",
        1394620117,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "e7e5a2ba6a79cb518a1302b150e4ee1b088d6398",
        "Arcade Classic No. 3 - Galaga & Galaxian",
        "(Europe) (SGB)",
        "DMG-AGCP",
        3496689546,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "73add692525be0696bbca64a9d62825d22fb4f3d",
        "Mystical Ninja Starring Goemon",
        "(USA) (SGB)",
        "DMG-AGDE",
        4210766908,
        262144,
        "En",
        "USA"
   , " " },
    {
        "685edaa81085d5a7897791bdd32910eecdb8bbef",
        "Ganbare Goemon - Kurofunetou no Nazo",
        "(Japan) (SGB)",
        "DMG-AGDJ",
        2433416740,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "207c55c4f008eccbcdbb0da58f1af2b9cee65ece",
        "Mystical Ninja Starring Goemon",
        "(Europe) (SGB)",
        "DMG-AGDP",
        2709601493,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "6949a7b5abbce2dfda5871166cca69bcf7f87e1e",
        "Buster Bros.",
        "(USA)",
        "DMG-AGE",
        3022281891,
        131072,
        "En",
        "USA"
   , " " },
    {
        "e2902c6d46dad71de3978a7caef2eadf89957786",
        "Getaway, The - High Speed II",
        "(USA)",
        "DMG-AGEE",
        2402022679,
        262144,
        "En",
        "USA"
   , " " },
    {
        "1c4a441045c37003134cccda17a10fc27d7ce46f",
        "Sports Illustrated - Golf Classic",
        "(USA) (SGB)",
        "DMG-AGFE",
        2276656820,
        262144,
        "En",
        "USA"
   , " " },
    {
        "97545db83442927dfedfdf446470b271e543ed83",
        "Golf Classic",
        "(Europe) (SGB)",
        "DMG-AGFP",
        2799360798,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "587aefa58812cc10402965721b77446594464411",
        "Game Boy Gallery - 5 Games in 1",
        "(Europe) (SGB)",
        "DMG-AGGA",
        641716562,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "21e7c1be8f369135ae7794e889eb176557286def",
        "God Medicine - Fukkokuban",
        "(Japan) (SGB)",
        "DMG-AGHJ",
        2222851954,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "0c2cfbaeb1819793f0eb3a6a17c676bc86e0f87b",
        "Game Boy Gallery 2",
        "(Japan) (SGB)",
        "DMG-AGIJ",
        2472124803,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "cf19b8a1612442fbd0537b279af4348e228b6eb3",
        "Mach Go Go Go",
        "(Japan) (SGB)",
        "DMG-AGJJ",
        23929611,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "fa5873ff2be01ea4a64a78e3529698079152374b",
        "Genjin Cottsu",
        "(Japan) (SGB)",
        "DMG-AGKJ",
        2515074225,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "ab134196c4597baa03e2f9aad0a67330364caf53",
        "Game & Watch Gallery 2",
        "(USA, Europe) (SGB) (GB Comp.)",
        "DMG-AGLE",
        2526841185,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "a2f24f9e8dc03c7854b1e2bffffc0e9b79471e7a",
        "Game Boy Gallery 3",
        "(Australia) (SGB) (GB Comp.)",
        "DMG-AGLU",
        1765146273,
        1048576,
        "En",
        "Australia"
   , " " },
    {
        "621ef957ef099a6bfd1ddb2ef7b85fb606871b52",
        "Gamera - Daikaijuu Kuuchuu Kessen",
        "(Japan) (SGB)",
        "DMG-AGMJ",
        1651941057,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "5467fbf64ce65340696e1ef634faf3b4900eaa3f",
        "Nintama Rantarou GB",
        "(Japan) (SGB)",
        "DMG-AGNJ",
        3173134724,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "21ee321dbac437c660abcb083b3daf233fccd749",
        "Hugo",
        "(Europe) (SGB)",
        "DMG-AGOP",
        1957322255,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "0edb7d5c57519e8e0f7dbbdec3ff16e29c22d02d",
        "Puzzle Bobble GB",
        "(Japan)",
        "DMG-AGPJ",
        270753336,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "3162d1a9b4b6c94aea43d2e0a69ab3184bd89ba6",
        "Game & Watch Gallery 3",
        "(USA, Europe) (SGB) (GB Comp.)",
        "DMG-AGQE",
        449193434,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "06eec716433003b3287d53208a62372af5c4a482",
        "Game Boy Gallery 3",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AGQJ",
        345997438,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "ee37d53c110fcfe2d0ebf4ae0a22ad9fb84bcfcf",
        "Game Boy Gallery 4",
        "(Australia) (SGB) (GB Comp.)",
        "DMG-AGQU",
        625606110,
        1048576,
        "En",
        "Australia"
   , " " },
    {
        "eae85e25e9a835329782d89b81b8756aabc00393",
        "Honkaku Shougi - Shougi Ou",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AGSJ",
        982968424,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "cbe3b91cbaaff0244247cdd0df4f9d93e04321ca",
        "Stargate",
        "(USA, Europe)",
        "DMG-AGTE",
        1200171356,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "6ff0d2ae580bc36a9f0714d15fa4441ef135c348",
        "Ganbare Goemon - Tengutou no Gyakushuu",
        "(Japan) (GB Comp.)",
        "DMG-AGUJ",
        3626625839,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "7fa1ccb162f53a0290d8954ce8c43c2c78201860",
        "Great Battle Pocket, The",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AGVJ",
        3027353749,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "d2766346c0e38f0208e4e9a5aa4483c0e283eaf9",
        "Game Boy Wars Turbo",
        "(Japan) (SGB)",
        "DMG-AGWJ",
        2488677412,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "c2b83ea83e90aba875ee1d6d8263c16c40540f2c",
        "Pang",
        "(Europe)",
        "DMG-AGX",
        3935704500,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "23c5848125bab2e29e87652e0a789d1c1c67d268",
        "Game Boy Wars Turbo - Famitsu Version",
        "(Japan) (SGB)",
        "DMG-AGXJ",
        2261797883,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "a5cde1f461937bc3193d1acf616279a02b2ccaf9",
        "Nobunaga no Yabou - Game Boy Ban 2",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AGYJ",
        590897872,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "0fba8e2d6a96b21e4258f50c1d77eab7e8aae142",
        "Goukaku Boy Series - Koukou Nyuushi Derujun - Rekishi Nendai Anki Point 240",
        "(Japan)",
        "DMG-AH2J",
        2116430413,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "a7a32ec74beae81e3555efbb5c3c84a17184fe32",
        "Honkaku Hanafuda GB",
        "(Japan)",
        "CGB-AH3J",
        98352469,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "e455c9fa34f4c401ed49f0cc8dbafdeb13df5e8b",
        "Chase H.Q. - Secret Police",
        "(USA) (SGB) (GB Comp.)",
        "DMG-AH9E",
        2088754940,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "2f2e70da56c1b53932fc77b808cb2db2dfe04d45",
        "Chase H.Q. - Secret Police",
        "(Europe) (SGB) (GB Comp.)",
        "DMG-AH9P",
        3487531371,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "6f9e1571d8da2d7c3328024d8c0dc1e2b58708ea",
        "Monopoly",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AHAJ",
        84141415,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "16a2b055a9086537c26cea2602c46742829f5020",
        "Hello Kitty no Beads Factory",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AHBJ",
        318196948,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "3f2fb1d90737a7516a0f25f489a853f77c6f5aa9",
        "Hercules",
        "(USA, Europe) (SGB)",
        "DMG-AHCE",
        11075614,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "5f9508ba9c867392534bcaaf3f234afbd5a6e377",
        "International Track & Field",
        "(USA) (SGB) (GB Comp.)",
        "DMG-AHDE",
        2151505239,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "859144920831d73f96e7f32659ed684f9db7ac42",
        "Hyper Olympic Series - Track & Field GB",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AHDJ",
        2012704444,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "a70c659b74fbe56f52be01444cd51d06466e2576",
        "International Track & Field",
        "(Europe) (En,Fr,De,It) (SGB) (GB Comp.)",
        "DMG-AHDP",
        847169567,
        1048576,
        "En,Fr,De,It",
        "Europe"
   , " " },
    {
        "23f0cbaba1601ce8af0e624b4328bc1a75ecaf37",
        "Home Alone 2 - Lost in New York",
        "(USA, Europe)",
        "DMG-AHE",
        3907268849,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "bb41ea621418a88785978ac9143d6a6919bdbfa8",
        "Hot Wheels - Stunt Track Driver",
        "(USA, Europe) (SGB) (GB Comp.)",
        "DMG-AHEE",
        1923475488,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "12ce57724b66d7d6b5fd75c9b72fbd32ff4732ca",
        "GB Karan Koron Gakuen - Hanafuda Mahjong",
        "(Japan)",
        "CGB-AHFJ",
        2864324260,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "a4f535bd0013ec5396fd5e2506ca653ff4c5c6f1",
        "SD Hiryuu no Ken Gaiden",
        "(Japan) (SGB)",
        "DMG-AHGJ",
        3302004584,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "1e69d79ddb514e5afaee3415fd05922e5725d310",
        "Trade & Battle Card Hero",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AHHJ",
        2978789930,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "4fdeb19f864e16929f315dfc08a8dba69a4ba238",
        "Trade & Battle Card Hero",
        "(Japan) (Rev 1) (3DS Virtual Console) (SGB) (GB Comp.)",
        "DMG-AHHJ",
        3649734525,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "fb428d0e6fc05d695af744e0b4fb7be379e04134",
        "Halloween Racer",
        "(Europe) (En,Fr,De,Es,It,Pt)",
        "CGB-AHIP",
        1895019569,
        1048576,
        "En,Fr,De,Es,It,Pt",
        "Europe"
   , " " },
    {
        "6fca9819d1f2772983104ae78fc4f0787f863077",
        "Hello Kitty no Magical Museum",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AHKJ",
        4134963504,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "c119645f1b45be2c5582959871489882157c0ba3",
        "Hollywood Pinball",
        "(Europe) (En,Fr,De,It) (GB Comp.)",
        "DMG-AHLP",
        2345793077,
        1048576,
        "En,Fr,De,It",
        "Europe"
   , " " },
    {
        "4ea82e5cbddfe3184356ddf88beb58eb734a725e",
        "Hamster Paradise",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AHMJ",
        4112567065,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "ca8f53b455c6101c9855ad71966a69f5bed1cab1",
        "Hollywood Pinball",
        "(Japan) (GB Comp.)",
        "DMG-AHNJ",
        2010637751,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "dbef3e665caf9c6520902e0568270151941d2cd9",
        "Hugo 2 1-2",
        "(Germany) (GB Comp.)",
        "DMG-AHOP",
        4206250776,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "f233942cbb44c89da8477dcad85122b6351f3073",
        "Shanghai Pocket",
        "(USA) (SGB) (GB Comp.)",
        "DMG-AHPE",
        2783177961,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "b8e9a258c0ad3c75d383896e536b839190a474b5",
        "Shanghai Pocket",
        "(Europe) (SGB) (GB Comp.)",
        "DMG-AHPP",
        2483141191,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "da3285d410b7c2ed565d84070345030aa34c6ddf",
        "Shanghai Pocket",
        "(Europe) (Rev 1) (SGB) (GB Comp.)",
        "DMG-AHPP",
        3640312684,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "075f8a837246a111fb3f612681a5672a2329b15f",
        "Pocahontas",
        "(USA, Europe) (SGB)",
        "DMG-AHQE",
        3461924413,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "ac33301148bd6edc390df091b75fb34817d0967f",
        "Renju Club - Gomoku Narabe",
        "(Japan) (SGB)",
        "DMG-AHRJ",
        1391847776,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "0b9fcc04d8acffc8403397e6e47c43e833dc0d3c",
        "Hon Shougi",
        "(Japan) (SGB)",
        "DMG-AHSJ",
        1816166125,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "f8148e027d49b653d8a1a2ddd7548719bfe67887",
        "Hanasaka Tenshi Tenten-kun no Beat Breaker",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AHTJ",
        1855491079,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "84ea0bab17769e9231792f270af2bacea5c73deb",
        "SD Hiryuu no Ken Gaiden 2",
        "(Japan) (SGB)",
        "DMG-AHVJ",
        268651801,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "a219cabe6ebaeb9474cdfb3e8346ef11d876c433",
        "Pocket Sonar",
        "(Japan)",
        "DMG-AHWJ",
        3599540089,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "2c6048ef646fb5a7cc4530f725b5d18d18a3f15b",
        "From TV Animation Slam Dunk Limited Edition",
        "(Japan) (SGB)",
        "DMG-AHXJ",
        2908085232,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "436b51f4f726d4daef710cd5202eafaafc20bab8",
        "Super Mario Bros. Deluxe",
        "(USA, Europe) (Rev 1)",
        "CGB-AHYE",
        2427126907,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "92790dc82a6271d62916d9540f9a2d3ee1af90a2",
        "Super Mario Bros. Deluxe",
        "(Europe) (Rev 2)",
        "CGB-AHYE",
        1656467075,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "de1fb29548035bb828da2232d885453952844a75",
        "Super Mario Bros. Deluxe",
        "(USA, Europe)",
        "CGB-AHYE",
        2764908287,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "61c7ecb5395c1582ab399297b8fbe72a5e307640",
        "Super Mario Bros. Deluxe",
        "(Japan) (Rev 1) (NP)",
        "CGB-AHYJ",
        4108918627,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "e20f16bf6cddf021f66389e209a7910b13d557b6",
        "Super Mario Bros. Deluxe",
        "(Japan) (NP)",
        "CGB-AHYJ",
        2255163922,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "ba29db017c8603d6958fb3af0b27a9e4f6e81889",
        "Billy Bob's Huntin' 'n' Fishin'",
        "(USA, Europe)",
        "CGB-AHZE",
        4196296787,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "7274e819ce2ebcfd1aedfefe904ea5400012aa57",
        "Caesars Palace II",
        "(USA, Europe)",
        "CGB-AI2E",
        891004394,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "7fa040e3a34dd54f575d30cf465ced490853cf60",
        "Honkaku Taisen Shougi Ayumu",
        "(Japan) (GB Comp.)",
        "DMG-AIBJ",
        2299791689,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "0cba0aedf5d0caa0aab25ac997ef840f0375847b",
        "Hexcite - The Shapes of Victory",
        "(USA, Europe) (SGB) (GB Comp.)",
        "DMG-AICE",
        2215136863,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "b4c7c2bd859b8543c3957d3a0d0828dbf65eece4",
        "Glocal Hexcite",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AICJ",
        4052779921,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "462246476ffb0b7b3302f823e8db6bcb21040f01",
        "Initial D Gaiden",
        "(Japan) (SGB)",
        "DMG-AIDJ",
        1824876050,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "574918d111bec3a5553073dfe13c06066142dbf5",
        "Magical Chase GB - Minarai Mahoutsukai Kenja no Tani e",
        "(Japan)",
        "CGB-AIFJ",
        367383705,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "4a4510a59b890a9debed60096059c64a4bb848b4",
        "International Superstar Soccer",
        "(USA, Europe) (SGB)",
        "DMG-AIGE",
        2490727400,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "90f5d151666b5e1c337edf41f7afc6be14f89ddd",
        "World Soccer GB",
        "(Japan) (SGB)",
        "DMG-AIGJ",
        1473388276,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "62dbc8c722395534f759972b9d561035f5e7b0e4",
        "Doraemon no Study Boy 1 - Shou 1 Kokugo Kanji",
        "(Japan)",
        "DMG-AIKJ",
        1697332388,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "47ede5942cf8bf7891d75d8fb8187121aec8a28f",
        "Mission - Impossible",
        "(USA) (En,Fr,Es)",
        "CGB-AIME",
        1092816145,
        1048576,
        "En,Fr,Es",
        "USA"
   , " " },
    {
        "f74cc7e9da7c58a9ef324a82dab1af25fabbc8c5",
        "Mission - Impossible",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-AIMP",
        183708586,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "1fb41533b2629b57804c2e7a08525d6d13c7023e",
        "Battleship",
        "(USA, Europe) (GB Comp.)",
        "DMG-AIPE",
        2389671991,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "4f166fa644bc3591b4c7c89b57dd5705fe4757bd",
        "Puchi Carat",
        "(Europe) (SGB) (GB Comp.)",
        "DMG-AIQP",
        2846399962,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "b19f3afdad8c79d4d658075f7a3a209cf448f10e",
        "B-Daman Bakugaiden V - Final Mega Tune",
        "(Japan)",
        "CGB-AIRJ",
        1417321065,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "5c7f89b6f263ddb81267a0cb0fa5200eb0ec5b75",
        "Doraemon no Study Boy 2 - Shou 1 Sansuu Keisan",
        "(Japan)",
        "DMG-AISJ",
        1395904643,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "27f4256d7e4fcd4f29ea238ea69999d84edbec46",
        "Space Invaders",
        "(USA, Europe) (GB Comp.)",
        "DMG-AIVE",
        3669444108,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "79c9ce97cf9753cb72d76d5b112d6903101d8822",
        "Alice in Wonderland",
        "(USA, Australia)",
        "CGB-AIWE",
        832173663,
        2097152,
        "En",
        "USA, Australia"
   , " " },
    {
        "6e857be20f86ab7fcc44eef9c9c7dcb5f8076227",
        "Alice in Wonderland",
        "(Europe) (En,Fr,De,Es)",
        "CGB-AIWP",
        2236898689,
        2097152,
        "En,Fr,De,Es",
        "Europe"
   , " " },
    {
        "630d5305ce5a5c04acd90b7170adfc619f74e89b",
        "Mini 4 Boy II",
        "(Japan) (SGB)",
        "DMG-AJ2J",
        3600163393,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "1d60d24b25baf8971c7ae518e12b9ad05933479b",
        "Goukaku Boy Series - Z-Kai Reibun de Oboeru - Chuugaku Eitango 1132",
        "(Japan)",
        "DMG-AJ3J",
        3691056900,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "0693e650609d0a2ecbd706d10b4a0b34fb3a4494",
        "Mini 4 Boy",
        "(Japan) (SGB)",
        "DMG-AJ4J",
        102294704,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "63bf924928b806e201cb7f7ec7c525aa0310c577",
        "Goukaku Boy Series - Eijukugo Target 1000",
        "(Japan)",
        "DMG-AJ5J",
        3391145364,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "cc9357de9738055cfb69a138f2331fa7a51a5646",
        "Goukaku Boy Series - Shikakui Atama o Maruku Suru - Kokugo Battle Hen",
        "(Japan) (Imagineer)",
        "DMG-AJ6J",
        2346018769,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "09085ba8644fb6902e9c0526514e884e82037fa9",
        "NBA in the Zone 2000",
        "(USA)",
        "CGB-AJ7E",
        4256402505,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "32959d15bd6aa22c15707ee648f4299611562912",
        "NBA in the Zone 2000",
        "(Europe)",
        "CGB-AJ7P",
        3870256391,
        2097152,
        "En",
        "Europe"
   , " " },
    {
        "628f07f97b513aac121376309475f4fe04c951e6",
        "Animal Breeder",
        "(Japan) (SGB)",
        "DMG-AJAJ",
        3270427183,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "597bb877820027f8126a2fb9d62604295e30133a",
        "Jelly Boy",
        "(Europe)",
        "DMG-AJBE",
        2120587451,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "7cd469ebecdd3db777cd4d3777d3583e58e1347e",
        "Chiki Chiki Tengoku",
        "(Japan)",
        "DMG-AJCJ",
        3946051073,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "9f21bf1ff5507a9890fdbaafe245c43513d25f93",
        "Judge Dredd",
        "(USA, Europe)",
        "DMG-AJDE",
        3117487702,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "48c80678f33660a5fa6b0c8cea8ee77d79e8c91e",
        "Judge Dredd",
        "(Japan)",
        "DMG-AJDJ",
        2501613831,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "d46b06cf50b4e1f19a26b9cc1b3811d92da189c3",
        "J.League Excite Stage Tactics",
        "(Japan)",
        "CGB-AJEJ",
        1435452977,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "376ace71701999fd299416347b20349f99311265",
        "Jungle Strike",
        "(USA)",
        "DMG-AJGE",
        2056051688,
        262144,
        "En",
        "USA"
   , " " },
    {
        "254cdf856dd41f49aafd22b0b926cc3f6ba3e383",
        "Jungle Strike",
        "(Europe)",
        "DMG-AJGP",
        1983560379,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "b88c53a877147c34a892dde411a9362146d8ee71",
        "Go! Go! Hitchhike",
        "(Japan) (SGB)",
        "DMG-AJHJ",
        2220307934,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "95244b265d77a076f113c5a790dcb271962b2675",
        "Konami GB Collection Vol.1",
        "(Japan) (SGB)",
        "DMG-AJIJ",
        2569852815,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "a9612cb0891dd8aedc14cb73d1cc009ca16fd46e",
        "Quiz Nihon Mukashibanashi - Athena no Hatena",
        "(Japan)",
        "DMG-AJJ",
        1193026427,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "9c296f4e8e93e965f959a40b2d3a6d8eeb954899",
        "Konami GB Collection Vol.2",
        "(Japan) (SGB)",
        "DMG-AJJJ",
        3283294413,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "f26c317d4ba883bd5fe1c6f9748adb6deda931de",
        "Konami GB Collection Vol.3",
        "(Japan) (SGB)",
        "DMG-AJKJ",
        501861113,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "ac1c800dcb6d3107de082dc058dc383380be6ce8",
        "Konami GB Collection Vol.4",
        "(Japan) (SGB)",
        "DMG-AJLJ",
        977529395,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "c034bda813d7c8099aaadec9135fb6124e4e61cc",
        "NBA Jam 99",
        "(USA, Europe) (GB Comp.)",
        "DMG-AJME",
        2227048173,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "8fe31c1d32d75c10ced75f1dd2dafbde696b9f69",
        "Hamster Club",
        "(Japan) (GB Comp.)",
        "DMG-AJNJ",
        2320336544,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "43cd765e03ca704ccd30a60ab1f9e483347301af",
        "Janosch - Das grosse Panama-Spiel",
        "(Germany)",
        "CGB-AJOD",
        852028947,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "21b71fa4273e3dbc25886b89c885b01f0ee61adf",
        "Jeopardy! - Platinum Edition",
        "(USA) (SGB)",
        "DMG-AJPE",
        1376189705,
        131072,
        "En",
        "USA"
   , " " },
    {
        "85e60e20ae195b9d1f50526d15ed7d868bf0c643",
        "Jeopardy! - Teen Tournament",
        "(USA) (SGB)",
        "DMG-AJQE",
        3443388609,
        131072,
        "En",
        "USA"
   , " " },
    {
        "f4f3e326649be3de06928394ac7e4f1c6522ece1",
        "Super Pachinko Taisen",
        "(Japan) (SGB)",
        "DMG-AJRJ",
        3454074624,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "8fcf6de6a4722240f508073b1899ddeeb2d80c6d",
        "Matthias Sammer Soccer",
        "(Germany) (SGB)",
        "DMG-AJSD",
        1662936431,
        262144,
        "De",
        "Germany"
   , " " },
    {
        "74a540f64b834a04c285b287510e6c4867e87633",
        "J.League Big Wave Soccer",
        "(Japan) (SGB)",
        "DMG-AJSJ",
        3452972834,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "96f2fce9369e0eb38432f6c281a57d7339778933",
        "NBA Jam - Tournament Edition",
        "(USA, Europe)",
        "DMG-AJTE",
        668421200,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "e97a4a5295d70118a364bcc77735b648fff5856b",
        "NBA Jam - Tournament Edition",
        "(Japan)",
        "DMG-AJTJ",
        848338786,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "d7491bf6c0a5c701237a155bb2eb2432cd88b7d7",
        "Jeremy McGrath Supercross 2000",
        "(USA, Europe)",
        "CGB-AJUE",
        4042894310,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "b30f30737244f9a4c4fd69ab6765cb809eab294f",
        "Pocket Pro Wrestling - Perfect Wrestler",
        "(Japan)",
        "CGB-AJVJ",
        3572218433,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "2e1126fec13e39f0aabc3d6c9922380faf0dbd03",
        "Lil' Monster",
        "(USA) (SGB) (GB Comp.)",
        "DMG-AJYE",
        3330644788,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "c87f6cfc9f04e839917c11c41a78a39c4ae53fdf",
        "Gem Gem Monster",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AJYJ",
        3732211136,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "014fc87e13d06aaf0cbe2afb0ab60413bf031733",
        "Jeff Gordon XS Racing",
        "(USA) (GB Comp.)",
        "DMG-AJZE",
        2753485094,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "13aead5a4d48f59f45d8af929fb46a798e211589",
        "Mortal Kombat & Mortal Kombat II",
        "(USA, Europe)",
        "DMG-AK2E",
        1429212426,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "adf7b2f17cc39fe1c3ce26d3037c4c57076fef2a",
        "Mortal Kombat & Mortal Kombat II",
        "(Japan)",
        "DMG-AK2J",
        2980634576,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "00297dfa9a12f4bac601d70debc4bb8e9500ea1c",
        "Pachi-Slot Kids 3",
        "(Japan)",
        "DMG-AK3J",
        1090261382,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "228f33e179baf0b56baf8e7381a47e69314f2503",
        "Goukaku Boy Series - Z-Kai Kyuukyoku no Eigo Koubun 285",
        "(Japan)",
        "DMG-AK4J",
        3616732999,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "77826314e3c98263a88d658fd85ddb1367100230",
        "Crayon Shin-chan - Ora no Gokigen Collection",
        "(Japan) (SGB)",
        "DMG-AK5J",
        3698473630,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "4bc1caaccf99c9ace9e14708ce3e71ad657d93bd",
        "Nettou The King of Fighters '96",
        "(Japan) (SGB)",
        "DMG-AK6J",
        4274892354,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "8ccd9bd90e5a72da249aaf3bbd84f36de52dac7a",
        "King of Fighters, The - Heat of Battle",
        "(Europe) (SGB)",
        "DMG-AK6P",
        1294830531,
        524288,
        "En",
        "Europe"
   , " " },
    {
        "4a8db8ecb816032c7b7d63e44189716b765eabca",
        "Goukaku Boy Series - Gakken - Kanyouku Kotowaza 210",
        "(Japan)",
        "DMG-AK7J",
        1156949059,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "0b0fea5f2347e3e53bd6473f20bcad0904b61692",
        "Goukaku Boy Series - Shikakui Atama o Maruku Suru - Shakai Battle Hen",
        "(Japan) (Imagineer)",
        "DMG-AK8J",
        2280705965,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "e43e4d847513ad71b2d03896d86fff7fb0b76563",
        "GBKiss Mini Games",
        "(Japan)",
        "DMG-AKAJ",
        2459975619,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "3af657f65324ad148695696f4940f56087ffe92c",
        "Kirby's Dream Land 2",
        "(USA, Europe) (SGB)",
        "DMG-AKBE",
        2378202165,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "3193c8b37d17d82a2dfeb3c154d182a5e8e88f5c",
        "Hoshi no Kirby 2",
        "(Japan) (SGB)",
        "DMG-AKBJ",
        1273885228,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "238824debe4256230a14b796efc0126bc55a2851",
        "Kirby's Star Stacker",
        "(USA, Europe) (SGB)",
        "DMG-AKCE",
        2435844248,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "3d6fb5def71733de4829f8daec1022c8ef9efe4a",
        "Kirby no Kirakira Kids",
        "(Japan) (SGB)",
        "DMG-AKCJ",
        1207185218,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "991cd78ef71f41dbbf413a0431d7b69741b59625",
        "Karamuchou no Daijiken",
        "(Japan) (SGB)",
        "DMG-AKDJ",
        4202604704,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "cc6dea9466b8025d1f3e6f6d255b3a8c45b692b9",
        "Attack of the Killer Tomatoes",
        "(USA, Europe)",
        "DMG-AKE",
        3048441952,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "9eae4a612107a7ee75b6bd2bc71729263c4e615e",
        "Koukiatsu Boy",
        "(Japan) (SGB)",
        "DMG-AKEJ",
        1409253441,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "7b55932bd42ab4b0ee9cebf4e12a651f7b37525b",
        "King of Fighters '95, The",
        "(USA) (SGB)",
        "DMG-AKFE",
        536366732,
        524288,
        "En",
        "USA"
   , " " },
    {
        "6d2a0d13434f8be332a61a61a18cf222e75f650b",
        "Nettou The King of Fighters '95",
        "(Japan) (SGB)",
        "DMG-AKFJ",
        2266165627,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "4a8653665622827a20dbe678e07b4d94b2456e86",
        "King of Fighters '95, The",
        "(Europe) (SGB)",
        "DMG-AKFP",
        2750001480,
        524288,
        "En",
        "Europe"
   , " " },
    {
        "0effe427633c69d043d574d55c66d54e5002293d",
        "Ken Griffey Jr. Presents Major League Baseball",
        "(USA, Europe) (SGB)",
        "DMG-AKGE",
        177756609,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "e482778b6404523f8a07f9b6f018db16059fb24d",
        "Katou Hifumi Kudan - Shougi Kyoushitsu",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AKHJ",
        2724341407,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "cfc594dcb29c5ccee91e5e7aec2ffacbde3d61f7",
        "Attack of the Killer Tomatoes",
        "(Japan)",
        "DMG-AKJ",
        587630201,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "db44ec365b1a1b569a61282ac55c9ca160ccf1f1",
        "Konchuu Hakase",
        "(Japan) (Rev 1) (SGB)",
        "DMG-AKJJ",
        91568016,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "eb5a87ac005e6f1ea5a26af9631139d01a8dd033",
        "Konchuu Hakase",
        "(Japan) (SGB)",
        "DMG-AKJJ",
        4138984081,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "641548f10bfabb80201202d3ead1c056b4a818a2",
        "Doraemon no Study Boy 3 - Ku Ku Master",
        "(Japan)",
        "DMG-AKKJ",
        56009993,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "0a55fb16901615dc783521af0fbc06c33aa633e5",
        "Killer Instinct",
        "(USA, Europe) (SGB)",
        "DMG-AKLE",
        2893626196,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "4db6953cf679f21acac9c137bc730edbbbc67062",
        "Kandume Monsters",
        "(Japan) (SGB)",
        "DMG-AKMJ",
        2195734032,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "ba62b86660544754a23659e1bb146eda8305a322",
        "Doraemon no Study Boy 6 - Gakushuu Kanji Master 1006",
        "(Japan)",
        "DMG-AKNJ",
        1511442573,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "cd38a2704d50f2f87e78e4e1d5aa9f57cd5002b8",
        "Pocket Bomberman",
        "(Europe) (SGB)",
        "DMG-AKOE",
        265399892,
        524288,
        "En",
        "Europe"
   , " " },
    {
        "6e3de70e71457a2110c030c04ff9318328d6d940",
        "Kuma no Puutarou - Takara Sagashi da Ooiri Game Battle!",
        "(Japan) (SGB)",
        "DMG-AKPJ",
        4028435335,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "c69bea172debf9bedf16989c166fb0c3ddb37cff",
        "Pocket Bomberman",
        "(USA, Europe) (SGB) (GB Comp.)",
        "DMG-AKQE",
        4197082857,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "7dec81cc2f8e860de8d725db5929eb9c7f3b2699",
        "Kaseki Sousei Reborn",
        "(Japan) (SGB)",
        "DMG-AKRJ",
        1122548361,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "a87e065df05414cb07e04af62b90b97cf6983192",
        "Kaseki Sousei Reborn",
        "(Japan) (Rev 1) (SGB)",
        "DMG-AKRJ",
        2090080636,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "24c09176f58939af2608623e13d4ccd6bc851170",
        "Koushien Pocket",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AKSJ",
        1762721594,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "70c25353fc04af8fc74e71013f9d78b5a0ea7923",
        "Gegege no Kitarou - Youkai Souzoushu Arawaru!",
        "(Japan) (SGB)",
        "DMG-AKTJ",
        686098352,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "21443311d7e4cc5a977cae03d13ca436949ba4f2",
        "Klustar",
        "(USA) (GB Comp.)",
        "DMG-AKUE",
        1066229825,
        262144,
        "En",
        "USA"
   , " " },
    {
        "2310216e1bab6edd08ba81ed49dc79f0d0305172",
        "Klustar",
        "(Europe) (En,Fr,De,Es,It) (GB Comp.)",
        "DMG-AKUP",
        1467880737,
        262144,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "46-45537",
        "Pokemon Picross",
        "(Japan) (Proto)",
        "DMG-AKVJ",
        3479469899,
        2097152,
        "Ja",
        "Japan",
        "/pok_picross64.sprite" 
    },
    {
        "39-60904",
        "Pokemon Crystal Clear",
        "(Fanmod)",
        "BYTE",
        3479469899,
        2097152,
        "En",
        "World",
        "/cry_clear64.sprite" 
    },
    {
        "e5ce4a8d44fa02060c98126eeaca1c3bf8ee4228",
        "Carmageddon - Carpocalypse Now",
        "(Germany)",
        "CGB-AKWD",
        3024600070,
        2097152,
        "De",
        "Germany"
   , " " },
    {
        "e8b888f92869eb51cabe2e4a74afc281cf50c2f3",
        "Carmageddon - Carpocalypse Now",
        "(USA, Europe) (En,Fr,Es,It)",
        "CGB-AKWP",
        3142069975,
        2097152,
        "En,Fr,Es,It",
        "USA, Europe"
   , " " },
    {
        "641ea27914a4d33da09462b300bbaaa92f5595ee",
        "Kirby's Block Ball",
        "(USA, Europe) (SGB)",
        "DMG-AKXE",
        2061044555,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "ab13b011792d09d69ed3615b3284c4ed16237d4b",
        "Kirby no Block Ball",
        "(Japan) (SGB)",
        "DMG-AKXJ",
        3745234135,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "e15c0bda808c067890bb9a4f8ded6c718842d47c",
        "Pocket Kyoro-chan",
        "(Japan) (SGB)",
        "DMG-AKYJ",
        3224693985,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "53456be7c9ffc7a7d9643414bd0cebe901237b21",
        "Pachinko CR Daiku no Gen-san GB",
        "(Japan) (SGB)",
        "DMG-AKZJ",
        1772480129,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "697e62936fc72981c7fbe60920070e89b7513106",
        "Lemmings 2 - The Tribes",
        "(Europe)",
        "DMG-AL2E",
        2550185289,
        524288,
        "En",
        "Europe"
   , " " },
    {
        "f36750fd6630f6b5ebe4cfffdb13c9357a58f548",
        "Super Robot Taisen - Link Battler",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AL6J",
        3528349997,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "94b5bd463a401df609104902e83d3fc0e2b74beb",
        "Soul Getter - Houkago Bouken RPG",
        "(Japan) (GB Comp.)",
        "DMG-AL9J",
        1073534043,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "9c76fd0cbcb65f9c6bd473ede71bac08765e71f3",
        "Altered Space - A 3-D Alien Adventure",
        "(Japan)",
        "DMG-ALA",
        3459703662,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "4a966c494e12256215ac6c99d6bd5b82ae74bc07",
        "Aladdin",
        "(USA) (SGB)",
        "DMG-ALAE",
        2943081552,
        262144,
        "En",
        "USA"
   , " " },
    {
        "45477da4567e3859600eb4397ecba989717f0759",
        "Revelations - The Demon Slayer",
        "(USA) (SGB) (GB Comp.)",
        "DMG-ALBE",
        3517341044,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "99d08aa3af74a953ba38a2b4b8c0c704626369e7",
        "Megami Tensei Gaiden - Last Bible",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-ALBJ",
        3181094457,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "c342711a48aee5f65c940a923e1c4564cfc24365",
        "Missile Command",
        "(Europe)",
        "CGB-ALCP",
        404844070,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "057e246a9c4311693a4194bcba526fd978b85ad6",
        "Super Chinese Land 1-2-3'",
        "(Japan) (SGB)",
        "DMG-ALDJ",
        2099089372,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "5729ba0978e01cce91c389537e1b24798870c7bd",
        "Altered Space - A 3-D Alien Adventure",
        "(USA)",
        "DMG-ALE",
        337016275,
        131072,
        "En",
        "USA"
   , " " },
    {
        "191762ce9db97e5b9a11e253e726e06aa7a1b917",
        "Lemmings & Oh No! More Lemmings",
        "(USA)",
        "CGB-ALEE",
        2548420143,
        4194304,
        "En",
        "USA"
   , " " },
    {
        "16bdb29415e1460072a3e9b89345310911cdd9a0",
        "Lemmings VS",
        "(Japan)",
        "CGB-ALEJ",
        2491237806,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "7f07e2ee95314348f9c339bf5bc79227e03f5d7e",
        "Black Bass - Lure Fishing",
        "(USA, Europe) (GB Comp.)",
        "DMG-ALFE",
        3830020060,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "15cc5e4f66e477c84d8f3d371c25d982504216fc",
        "Alien Olympics",
        "(Europe)",
        "DMG-ALGP",
        1480330830,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "5c00cf9c15e9a70663643cb9c41d05daca47ab61",
        "Towers - Lord Baniff's Deceit",
        "(USA, Europe)",
        "CGB-ALHE",
        2073764968,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "03a2f17bfe5a07dd855d95722404df3d40500775",
        "Ballistic",
        "(USA) (GB Comp.)",
        "DMG-ALIE",
        2835681138,
        524288,
        "En",
        "USA"
   , " " },
    {
        "cc10a49e132abf6e1279d2284e91238116ea5e16",
        "Little Magic",
        "(Japan)",
        "CGB-ALJJ",
        3392677949,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "a030a6d039cf6a3e46f150c9636341f5b39ad9e1",
        "Tottemo! Lucky Man - Lucky Cookie Minna Daisuki!!",
        "(Japan) (SGB)",
        "DMG-ALKJ",
        2409506989,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "01eaafbfb0a37c82aa6084ba39f9bb6357d826a9",
        "Lucky Luke",
        "(Europe) (En,Fr,De,Es)",
        "DMG-ALLP",
        1401816465,
        262144,
        "En,Fr,De,Es",
        "Europe"
   , " " },
    {
        "919676ed9593ff5e7e59ecdb6c3a7d638d614283",
        "Looney Tunes Collector - Alert!",
        "(USA) (En,Fr,Es)",
        "CGB-ALME",
        1414569252,
        2097152,
        "En,Fr,Es",
        "USA"
   , " " },
    {
        "9a8b44bbf2ae5b9acee7168fc38bdcdbfe6b672a",
        "Looney Tunes Collector - Martian Quest!",
        "(Japan)",
        "CGB-ALMJ",
        3158745810,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "6c7527ea4faf345cf3fd9bb0bf6f20d5cfb37b60",
        "Looney Tunes Collector - Martian Alert!",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-ALMP",
        1569054637,
        2097152,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "acd4b44d2ec5eb800e2869e024f97355aee0f671",
        "Lion King, The",
        "(USA)",
        "DMG-ALNE",
        875614817,
        524288,
        "En",
        "USA"
   , " " },
    {
        "55165e613b1ba59173d867fe15b1efc9ef63bde2",
        "Lion King, The",
        "(Europe)",
        "DMG-ALNP",
        2411973235,
        524288,
        "En",
        "Europe"
   , " " },
    {
        "8d30961379e5a278540658e609761bdf6059b646",
        "Lucky Luke",
        "(USA) (En,Fr,De,Es)",
        "CGB-ALQE",
        2783092532,
        1048576,
        "En,Fr,De,Es",
        "USA"
   , " " },
    {
        "fc5d57af171dae425f4a3d647e52435278d65733",
        "Lucky Luke",
        "(Europe) (En,Fr,De,Es)",
        "CGB-ALQP",
        1093645692,
        1048576,
        "En,Fr,De,Es",
        "Europe"
   , " " },
    {
        "146d00e2ef5a7d0acc32b10a9a5d07a0c9fc9bf5",
        "Looney Tunes",
        "(USA) (GB Comp.)",
        "DMG-ALTE",
        1324604887,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "7dbf60b34b37fffa8e12b951a5ef6e9928701665",
        "Looney Tunes",
        "(Europe) (GB Comp.)",
        "DMG-ALTP",
        1993293370,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "fab23aa8f0f7f79dee2658540e4f0d7acae94042",
        "Tabaluga",
        "(Germany) (GB Comp.)",
        "DMG-ALUP",
        4036989655,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "af68f9094a4e2a0e3c8c48ae5d421b40912ac009",
        "Sylvanian Families - Otogi no Kuni no Pendant",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-ALVJ",
        3338552471,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "d6c58407d7da5d1e4e8ec4308d0f1b38daf2ca65",
        "Lost World, The - Jurassic Park",
        "(USA, Europe) (SGB)",
        "DMG-ALWE",
        958354218,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "6ec5e13c534432895ea347f16bd67e45eb2cfde1",
        "Monde Perdu, Le - Jurassic Park",
        "(France) (En) (SGB)",
        "DMG-ALWF",
        4213364526,
        524288,
        "En",
        "France"
   , " " },
    {
        "591d853feb37ac100150fa0ce0731f80e35b795a",
        "Altered Space - A 3-D Alien Adventure",
        "(Europe)",
        "DMG-ALX",
        618002309,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "623c0849d9e055114c66256db2c10a9b264b84fd",
        "Klax",
        "(USA, Europe)",
        "CGB-ALXE",
        1904331728,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "32dce365ae7cbc88803c97bf765c01e217e34828",
        "Monopoly",
        "(Europe) (En,Fr,De)",
        "DMG-ALYP",
        2558222878,
        262144,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "42e60f4fb3884e48afd87cd55f7e3320c0c01fd6",
        "Speedy Gonzales - Aztec Adventure",
        "(USA, Europe) (GB Comp.)",
        "DMG-ALZE",
        2927800228,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "b94c95d609c1baba682d8f52a24fc5bc3d62c533",
        "Momotarou Dengeki 2",
        "(Japan) (SGB)",
        "DMG-AM2J",
        1008492212,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "70f55acd3a356f6cc4c352c199de5d4005d479ea",
        "Momotarou Collection 2",
        "(Japan) (SGB)",
        "DMG-AM3J",
        1445760895,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "f3ea334824ea6f5de0a438a18420c8cea02bf32a",
        "Mini Yonku GB Let's & Go!!",
        "(Japan) (SGB)",
        "DMG-AM4J",
        4228651019,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "4891cca3b77a7eb69ad23afe205fccb18b76e13f",
        "Goukaku Boy Series - Shikakui Atama o Maruku Suru - Suuji de Asobou Sansuu Hen",
        "(Japan)",
        "DMG-AM5J",
        4047614684,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "5aa84b88f4a87c87e293d31473634cf0939f4f15",
        "Goukaku Boy Series - Shikakui Atama o Maruku Suru - Suuji de Asobou Sansuu Hen",
        "(Japan) (Rev 1)",
        "DMG-AM5J",
        1954681416,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "d8e51d658ef267fc3971c72e5d95bbdb65ca9b1c",
        "Daikaijuu Monogatari - The Miracle of the Zone II",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AM6J",
        1764583316,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "061d4ef61c3170b688f8a89bca3da2aaea648993",
        "Cardcaptor Sakura - Itsumo Sakura-chan to Issho",
        "(Japan) (Rev 2) (GB Comp.)",
        "DMG-AM7J",
        1917051480,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "67f1830217552d08033ec97522a23c8d0e03f476",
        "Cardcaptor Sakura - Itsumo Sakura-chan to Issho",
        "(Japan) (Rev 1) (GB Comp.)",
        "DMG-AM7J",
        1139969570,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "c48d6ae5220f05d656d256583608e88c58f540de",
        "Cardcaptor Sakura - Itsumo Sakura-chan to Issho",
        "(Japan) (GB Comp.)",
        "DMG-AM7J",
        2310691053,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "6cb1d468660d8e5c28911b80126e1142ee0cc72a",
        "Moomin's Tale",
        "(Europe) (En,Fr,De)",
        "CGB-AM9E",
        1163148194,
        1048576,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "f19422ccb7e621de7e67eed63acd130a2f233a04",
        "Amida",
        "(Japan)",
        "DMG-AMA",
        1611827880,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "60b7186ddd4ff9404e5f22f54576ff410b2ce6f8",
        "Marmalade Boy",
        "(Japan) (SGB)",
        "DMG-AMAJ",
        255850458,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "af4a3d5326c29705d9930e5474a5a9e0f6ebe7b9",
        "Arcade Classic No. 1 - Asteroids & Missile Command",
        "(USA, Europe) (SGB)",
        "DMG-AMCE",
        2186914900,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "f72ea18fac7569c1169132e5adbe7012acf54279",
        "Momotarou Collection",
        "(Japan) (SGB)",
        "DMG-AMDJ",
        2906089733,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "34cea5a75a7c8d0dbfc0171c9eeb25eb793a1188",
        "Money Idol Exchanger",
        "(Japan) (SGB)",
        "DMG-AMEJ",
        2147624617,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "c12ae0c05ef4ac545304d9c6112d3dacd9f54a73",
        "Monster Race Okawari",
        "(Japan) (SGB)",
        "DMG-AMFJ",
        3174054373,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "d0728b9d75b33415bdaad972d3e022d8ddc86251",
        "Mahoujin GuruGuru - Yuusha to Kukuri no Daibouken",
        "(Japan) (SGB)",
        "DMG-AMGJ",
        759405837,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "568b52250e49d71975283fbcfcc851968692013f",
        "Motocross Maniacs 2",
        "(USA)",
        "CGB-AMHE",
        399671209,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "3e9a68cd11b083ccde2c624da5f40c2c8faf10cb",
        "Crazy Bikers",
        "(Europe)",
        "CGB-AMHP",
        1407200135,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "1ef120419755167c1c2833762e1824b40b4e6d4e",
        "Maui Mallard in Cold Shadow",
        "(USA)",
        "DMG-AMIE",
        1865458746,
        262144,
        "En",
        "USA"
   , " " },
    {
        "0c1e6539e91f93493d57a266639a92f1947d306c",
        "Mahjong Quest",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AMJJ",
        3362830479,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "0ad989c34b97bb8fbb79b4e63273f68070069e1b",
        "Mortal Kombat II",
        "(USA, Europe)",
        "DMG-AMKE",
        3215896016,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "24e01313e990da45aadfd9667e3957afbc2de0ab",
        "Mortal Kombat II - Kyuukyoku Shinken",
        "(Japan)",
        "DMG-AMKJ",
        1471831382,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "18573facf23834aa9218a20300adaa11ac23d80d",
        "Mulan",
        "(USA) (SGB)",
        "DMG-AMLE",
        3218043297,
        524288,
        "En",
        "USA"
   , " " },
    {
        "c5251b92c2dc6f257ab8452c8e7e7e3fb674e6ee",
        "Mulan",
        "(Europe) (SGB)",
        "DMG-AMLP",
        3800420144,
        524288,
        "En",
        "Europe"
   , " " },
    {
        "295c64cde5f9e7990d0f8cdc691b28be668163ec",
        "Mogu Mogu Gombo - Harukanaru Chou Ryouri Densetsu",
        "(Japan) (SGB)",
        "DMG-AMMJ",
        2244576498,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "7ce257a42a8ac286fdd511d35b59f26bf185003c",
        "Men in Black - The Series",
        "(USA, Europe) (SGB) (GB Comp.)",
        "DMG-AMNE",
        1706603331,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "fe89a28f5f9dad463cea233afbddcda2314c0cbb",
        "Mole Mania",
        "(USA, Europe) (SGB)",
        "DMG-AMOE",
        741787468,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "db5937e4578de72aebb92cf31aaaaae603e47cea",
        "Moguranya",
        "(Japan) (SGB)",
        "DMG-AMOJ",
        2197594628,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "9a348617123db1e6779fd366853c45d124aae516",
        "Medarot - Parts Collection",
        "(Japan) (SGB)",
        "DMG-AMPJ",
        4106925462,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "be34ccc6dff537a406fad6a1441466fd5044217a",
        "Oyatsu Quiz Mogu Mogu Q",
        "(Japan) (SGB)",
        "DMG-AMQJ",
        2044745609,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "3c6d223bb11c528e1d8e1aca0a0bd9177eaaf710",
        "Monster Race",
        "(Japan) (SGB)",
        "DMG-AMRJ",
        3247553894,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "373a8857c30a6317c94fb3af1e26cbb6b0bfd404",
        "Shougi Saikyou",
        "(Japan) (Rev 1) (SGB)",
        "DMG-AMSJ",
        2692005758,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "bac3276b77db754398af636c7de5920eb894ebbd",
        "Shougi Saikyou",
        "(Japan) (SGB)",
        "DMG-AMSJ",
        789544046,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "150c3ef058f563b6b17a19902c3e200914ab9174",
        "Montezuma's Return!",
        "(Europe) (En,Fr,De,Es,It)",
        "DMG-AMTP",
        3886814555,
        262144,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "9225e87b4364403b485dfadd47018f0aaba70337",
        "Nichibutsu Mahjong - Yoshimoto Gekijou",
        "(Japan)",
        "DMG-AMUJ",
        3649306440,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "bf31bfa0f963cfda1eb2c628f81cb665b0661a79",
        "Maya the Bee & Her Friends",
        "(Europe) (En,Fr,De) (GB Comp.)",
        "DMG-AMVP",
        2554010918,
        1048576,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "797e10ad002fdaa016b00687205b4aaf75ef0610",
        "Monster Race 2",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AMWJ",
        3112562904,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "727983c952ac4a8395f89636f6434e0e73ed7eed",
        "Medarot - Kabuto Version",
        "(Japan) (Rev 1) (SGB)",
        "DMG-AMXJ",
        312711213,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "b47f9d12b8ad9fcf7d767b066c7444d1f4c251c0",
        "Medarot - Kabuto Version",
        "(Japan) (SGB)",
        "DMG-AMXJ",
        464230656,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "f83c9f6192bfcbe17100acd40cc221e8f44b6134",
        "Medarot - Kuwagata Version",
        "(Japan) (Rev 1) (SGB)",
        "DMG-AMYJ",
        1584916526,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "fcf5e663bf22c7284fcf4c79cc76d317e1fb8829",
        "Medarot - Kuwagata Version",
        "(Japan) (SGB)",
        "DMG-AMYJ",
        3154479300,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "2a41968ad4669a719f6c42a9bb7fefadbada34e9",
        "Daikaijuu Monogatari - The Miracle of the Zone",
        "(Japan) (SGB)",
        "DMG-AMZJ",
        651351490,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "dd169d4384a5f936f2037ea418c82b32b604bcf1",
        "Namco Gallery Vol.2",
        "(Japan) (SGB)",
        "DMG-AN2J",
        2795213408,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "2ddea994233e1b509a872f12762ee7253eb7dea9",
        "Goukaku Boy Series - Nihonshi Target 201",
        "(Japan)",
        "DMG-AN3J",
        3474142073,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "8e7a9e2ea7cc87c8ee8e1f91a262213e565a4da2",
        "Goukaku Boy Series - Yamakawa Ichimonittou - Nihonshi B Yougo Mondaishuu",
        "(Japan)",
        "DMG-AN4J",
        2694584208,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "87e9104ef85965e80d4b2087b7f4bd7775feac6c",
        "Nectaris GB",
        "(Japan) (SGB)",
        "DMG-AN5J",
        549443567,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "28635cb449b1ee3134012e55a1dd280902b4308d",
        "Tales of Phantasia - Narikiri Dungeon",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AN6J",
        1918694172,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "cd6e0bd9032d32d0f241ce0a63679f94799ce44c",
        "Mr. Nutz",
        "(Europe)",
        "DMG-AN8E",
        62278197,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "59ef03de4aaf9359bb462e2f677a6b131a09db37",
        "Midori no Makibaou",
        "(Japan) (SGB)",
        "DMG-ANAJ",
        500776341,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "54fcbbb39e94f93f1e0096ad925205534116e9bc",
        "Doraemon no Study Boy 5 - Shou 2 Sansuu Keisan",
        "(Japan)",
        "DMG-ANBJ",
        1798960647,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "b6d266d7213fdd16ebbc062eaab4b69600c11d4f",
        "Animaniacs",
        "(USA) (SGB)",
        "DMG-ANCE",
        1732018525,
        262144,
        "En",
        "USA"
   , " " },
    {
        "99aa058f075f6d220c943e644ba6f79292dcb84e",
        "Animaniacs",
        "(Europe) (SGB)",
        "DMG-ANCP",
        2065948764,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "4e89b3511604051920cfd0847b52e984a3f15dec",
        "Hunchback of Notre Dame, The - 5 Foolishly Fun Topsy Turvy Games",
        "(USA, Europe) (SGB)",
        "DMG-ANDE",
        977680127,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "2016a91230f0565f7f0e82065d30fbe1cfb36477",
        "Asteroids",
        "(USA, Europe)",
        "DMG-ANE",
        3254290483,
        32768,
        "En",
        "USA, Europe"
   , " " },
    {
        "0c5f4dabdab1bf827c1a78b1fca6a2c09b88adc1",
        "NHL Blades of Steel",
        "(USA)",
        "CGB-ANEE",
        2828053938,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "df579792889084ce1d255d27dcb9e33ecc594149",
        "Namco Gallery Vol.1",
        "(Japan) (SGB)",
        "DMG-ANGJ",
        419612349,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "4a906a496dd1bc3f230e44f940eca8692f2a4c9c",
        "NHL Hockey 95",
        "(USA, Europe) (SGB)",
        "DMG-ANHE",
        3165377234,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "489f894bc814505673373e3d192d7d6d9631f78a",
        "Doraemon no Study Boy 4 - Shou ni Kokugo Kanji",
        "(Japan)",
        "DMG-ANJJ",
        1172564210,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "7452b7027e25d07fc3dfdeb23da9c87a10624d4e",
        "Ninku",
        "(Japan) (SGB)",
        "DMG-ANKJ",
        3036748267,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "a501d4323ec09a6e18f6bb8c3a9a27c56fc8b06e",
        "Madden 95",
        "(USA, Europe) (SGB)",
        "DMG-ANLE",
        3529006011,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "94b3cad14de0b62f48388472e1adad59c724b962",
        "Marble Madness",
        "(USA, Europe)",
        "CGB-ANNE",
        1321699205,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "a5d42d29054903d187a3837ec28c1026e53ecff4",
        "Monopoly",
        "(USA) (GB Comp.)",
        "DMG-ANOE",
        3847857943,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "cd06dbcc97980f0e207194ab8c72a886a3cc32cb",
        "Pachipachi Pachisurou - New Pulsar Hen",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-ANPJ",
        507788571,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "185-41157",
        "NHL 2000",
        "(USA, Europe) (SGB) (GB Comp.)",
        "DMG-ANRE",
        3143782697,
        1048576,
        "En",
        "USA, Europe",
        "/NHL_200064.sprite" },
    {
        "46dead9ad4f87d2246f9c555f06f6be60584cce5",
        "Ninku Dai-2-dan - Ninku Sensou Hen",
        "(Japan) (SGB)",
        "DMG-ANSJ",
        2139195250,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "5c0b67ab15c4ed2802962d2031216af5c9277570",
        "Nikkan Berutomo Club",
        "(Japan) (SGB)",
        "DMG-ANTJ",
        2906621213,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "24fe7becc94b19d3178ebce867be57040ea24fb8",
        "Mr. Nutz",
        "(USA) (En,Fr,Es)",
        "CGB-ANUE",
        1509324073,
        1048576,
        "En,Fr,Es",
        "USA"
   , " " },
    {
        "1b1abdcd5e38695d8e80bb65b0311f04adbaacb9",
        "Mr. Nutz",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-ANUP",
        1312278337,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "3e061e7600a714a9633456f3fc60ba66dd18031d",
        "NHL Blades of Steel 2000",
        "(USA)",
        "CGB-ANVE",
        2344990250,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "b30529a99a2ac3179901c098dfc378791472cafc",
        "Itsudemo! Nyan to Wonderful",
        "(Japan) (SGB)",
        "DMG-ANWJ",
        474822245,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "16e3c1dcb62064cebf7b8806b5af3a7ccfdb6694",
        "Antz",
        "(USA) (En,Fr,Es) (GB Comp.)",
        "DMG-ANXE",
        3691766841,
        1048576,
        "En,Fr,Es",
        "USA"
   , " " },
    {
        "4f67fc884330767ad584815c5c76eb7cd2482f69",
        "Antz",
        "(Europe) (En,Fr,De,Es,It,Nl) (GB Comp.)",
        "DMG-ANXP",
        1211752641,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "f9b05e38ba0bd22fd069429d1d2c8895f96ead51",
        "College Slam",
        "(USA)",
        "DMG-ANYE",
        2773067122,
        524288,
        "En",
        "USA"
   , " " },
    {
        "aaa591b94f0d2dfca31b94164733da012fcf7ab6",
        "NBA in the Zone",
        "(USA) (Rev 1) (SGB) (GB Comp.)",
        "DMG-ANZE",
        3197411188,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "ed7afba734613fcd6a38ad20ac3a1d9f1f747a34",
        "NBA Pro 99",
        "(Europe) (SGB) (GB Comp.)",
        "DMG-ANZP",
        1954899118,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "74f257e9805c7be0b9e7f84a8cd7e8763da73c2d",
        "Hugo 2",
        "(Germany)",
        "DMG-AO2P",
        1738117028,
        131072,
        "De",
        "Germany"
   , " " },
    {
        "ce96e742027d2694729e0c062a39b3fe9be25ef5",
        "David O'Leary's Total Soccer 2000",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-AO3P",
        3260998490,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "5b9645c796ccad6911131a885ddff101f6ca9b03",
        "Barca Total 2000",
        "(Europe) (En,Fr,De,Es,It,Nl,Ca)",
        "CGB-AO3X",
        3440766262,
        1048576,
        "En,Fr,De,Es,It,Nl,Ca",
        "Europe"
   , " " },
    {
        "abb57a59ba59bc5904eae03f157e5e4faa9a23b9",
        "Bugs Bunny in - Crazy Castle 4",
        "(USA)",
        "CGB-AO4E",
        2564554720,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "88136e135ddda767e98e49072269a704232c147a",
        "Bugs Bunny et le Chateau des Catastrophes",
        "(France) (En)",
        "CGB-AO4F",
        1839785707,
        1048576,
        "En",
        "France"
   , " " },
    {
        "71f82021b141d7231bcb888405dbb616d0affec2",
        "Bugs Bunny - Crazy Castle 4",
        "(Japan)",
        "CGB-AO4J",
        3594026666,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "a7d198502b8ec767220775e9d5c08b399cb06ec5",
        "Bugs Bunny in - Crazy Castle 4",
        "(Europe)",
        "CGB-AO4P",
        1393909344,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "dc9fc25626af6fa58bf0a80c836d75699d21e8ac",
        "Oni V - Oni wo Tsugumono",
        "(Japan) (SGB)",
        "DMG-AO5J",
        2936211776,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "7d1b8c5a71a0d49f2b67b81abea680ed24de083a",
        "Othello Millennium",
        "(Japan) (GB Comp.)",
        "DMG-AO7J",
        3347055990,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "0032c4b81660f5e74b884129d6b898038d49f0d1",
        "Olympic Summer Games",
        "(USA, Europe) (SGB)",
        "DMG-AO9E",
        3196173665,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "79beb35e3e4a1058e0b660a8cc9c3616b958f17d",
        "Grand Theft Auto",
        "(USA) (GB Comp.)",
        "DMG-AOAE",
        2454578022,
        4194304,
        "En",
        "USA"
   , " " },
    {
        "2b8d501026573ac00cda438e31abc9bcd40e8c7a",
        "Grand Theft Auto",
        "(Europe) (En,Fr,De,Es,It) (GB Comp.)",
        "DMG-AOAP",
        632984113,
        4194304,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "d90388a1d7e9f66211b9e63a45477d8c979e7476",
        "Otto's Ottifanten - Baby Bruno's Alptraum",
        "(Europe) (En,Fr,De,Es)",
        "DMG-AOBP",
        203590264,
        262144,
        "En,Fr,De,Es",
        "Europe"
   , " " },
    {
        "0d1488703499b2d1ffdffabecfcaab890ae5c6ff",
        "Oddworld Adventures",
        "(USA, Europe)",
        "DMG-AODE",
        1695596770,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "0f53b9d15703f06a4269174a0ee10f7c1d77b850",
        "Goraku Ou Tango!",
        "(Japan) (GB Comp.)",
        "DMG-AOEJ",
        2180727777,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "4cd8cf1349366443c46e2c06b178f42f68ab6a6e",
        "Logical",
        "(USA)",
        "CGB-AOGE",
        3597825527,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "63264eaf2e5d702ec739e01e0670683fbd966d7a",
        "Logical",
        "(Europe)",
        "CGB-AOGP",
        1592686276,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "768b4b55d6a7e4a77a217d8302efa052d1123006",
        "Golf de Ohasuta",
        "(Japan) (GB Comp.)",
        "DMG-AOHJ",
        942095325,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "5492291d7af83774f59c5fd2f08a586bb89df4d5",
        "Toobin'",
        "(USA)",
        "CGB-AOIE",
        1030445452,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "2cd43f079d3d8fa3affab663fc778e80807cd7a7",
        "B-Daman Bakugaiden - Victory e no Michi",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AOKJ",
        3721295574,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "2f8072ab4ed28017feb3f0349f280de8f4caf541",
        "Asterix & Obelix",
        "(Europe) (En,Fr,De,Es)",
        "CGB-AOLP",
        1713950190,
        1048576,
        "En,Fr,De,Es",
        "Europe"
   , " " },
    {
        "91479b32686e2753fafd1bdec3981b0012b1240b",
        "Game de Hakken!! Tamagotchi - Osutchi to Mesutchi",
        "(Japan) (SGB)",
        "DMG-AOMJ",
        2526336669,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "ed27d2aa3dd67324c19affd6b3326074802f12cd",
        "Beatmania GB",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AOOJ",
        228372885,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "bb780da74812868b34954a3bb4b1e9ffdbff9765",
        "Karamuchou wa Oosawagi! - Polinkies to Okashina Nakama-tachi",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AOPJ",
        3717496945,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "bd59484ed8949348bd0d59622598a358ba7bc4ea",
        "Toy Story 2",
        "(USA, Europe) (SGB) (GB Comp.)",
        "DMG-AOQE",
        1202637717,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "2107f30109028d5725cb50ff48771b757c4162fd",
        "Ronaldo V-Soccer",
        "(USA) (En,Fr,Es,Pt) (GB Comp.)",
        "DMG-AORE",
        3491778320,
        1048576,
        "En,Fr,Es,Pt",
        "USA"
   , " " },
    {
        "bc6a977b5e38f81b718b83b0683a4543181d75b4",
        "Ronaldo V-Football",
        "(Europe) (En,Fr,De,Es,It,Nl,Pt) (GB Comp.)",
        "DMG-AORP",
        2824257638,
        1048576,
        "En,Fr,De,Es,It,Nl,Pt",
        "Europe"
   , " " },
    {
        "57c3af4a6abb9c0cbfbce38b7826b7e50a88e223",
        "Ohasuta Yama-chan & Raymond",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AOSJ",
        2215572271,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "37cee5cc9280de8be4d1892c1fa2be6aed2c1902",
        "Super Breakout!",
        "(USA) (GB Comp.)",
        "DMG-AOTE",
        1391795381,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "266495d18ce90909925566ae128319725b49f931",
        "Super Breakout!",
        "(Europe) (En,Fr,De,Es,It,Nl) (GB Comp.)",
        "DMG-AOTP",
        1748210237,
        524288,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "d0fb3808b7b7d5513f4a6c6c6a19699fdb50ba87",
        "Pong - The Next Level",
        "(USA, Europe)",
        "CGB-AOVE",
        1198248861,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "7b6b4c15c88d377f23f54c5e7d69e843154fb05d",
        "Oddworld Adventures 2",
        "(USA) (En,Fr,De,Es,It) (GB Comp.)",
        "DMG-AOWE",
        1545997658,
        1048576,
        "En,Fr,De,Es,It",
        "USA"
   , " " },
    {
        "301cb3e9039fd0935021ce69230336817e249648",
        "Oddworld Adventures 2",
        "(Europe) (En,Fr,De,Es,It) (GB Comp.)",
        "DMG-AOWP",
        1266921807,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "19ee38b820f50d458dcf43c845c81c8370f465f9",
        "Armorines - Project S.W.A.R.M.",
        "(USA, Europe) (En,De)",
        "CGB-AOXE",
        3929775219,
        1048576,
        "En,De",
        "USA, Europe"
   , " " },
    {
        "dca8775e3924d83fb45ea2899e3a0ac6a7fd5af5",
        "Columns GB - Tezuka Osamu Characters",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AOYJ",
        4226145847,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "84b9be093ab7d211b56ceb7d83a387522fc28103",
        "Dogz",
        "(USA)",
        "CGB-AOZE",
        2822336899,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "83ae39c5ed31e3a2a5cae8e4855d28c7c79639d9",
        "Dogz",
        "(Europe)",
        "CGB-AOZP",
        3465400697,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "bf1061f31c0eb77ac7687736d45f18196b05af5c",
        "Picross 2",
        "(Japan) (SGB)",
        "DMG-AP2J",
        4121581826,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "a4879424dd4699ee3120a60d9e744de46b59ad46",
        "Pachio-kun 3",
        "(Japan)",
        "DMG-AP3J",
        3336404370,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "02b6e09d8c84b4c3916d1830c929e70bad7454cd",
        "Pachio-kun Game Gallery",
        "(Japan)",
        "DMG-AP4J",
        3192330472,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "428eb0b730916853553b228ef3e7640ea6e59209",
        "Pocket Shougi",
        "(Japan) (SGB)",
        "DMG-AP5J",
        799129062,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "7ef06e8acdd16a372710b106a813bd6090f140a6",
        "Pocket Golf",
        "(Japan)",
        "DMG-AP6J",
        1968263867,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "d82a9770c04f5cb749b91e6284d353bb8a51c2df",
        "Goukaku Boy Series - Z-Kai Reibun de Oboeru - Kyuukyoku no Kobun Tango",
        "(Japan)",
        "DMG-AP7J",
        2567318547,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "3d9efe47730443205e43f75d2bac319085d00c2c",
        "Pocket Densha 2",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AP8J",
        1294395520,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "2b45bb98a0e799fa77625e14202df081e3b0d2b6",
        "Jim Henson's Muppets",
        "(Europe) (En,Fr,De,Es,It,Nl,Sv) (GB Comp.)",
        "DMG-AP9P",
        839170348,
        4194304,
        "En,Fr,De,Es,It,Nl,Sv",
        "Europe"
   , " " },
    {
        "24-23772",
        "Pokemon - Rote Edition",
        "(Germany) (SGB)",
        "DMG-APAD",
        2300147749,
        1048576,
        "De",
        "Germany",
        "/pokemon_red64.sprite"
    },
    {
        "32-37350",
        "Pokemon - Red Version",
        "(USA, Europe) (SGB)",
        "DMG-APAE",
        2675957075,
        1048576,
        "En",
        "USA, Europe",
        "/pokemon_red64.sprite"
    },
    {
        "24-31484",
        "Pokemon - Version Rouge",
        "(France) (SGB)",
        "DMG-APAF",
        864013841,
        1048576,
        "Fr",
        "France",
        "/pokemon_red64.sprite"
    },
    {
        "24-35282",
        "Pokemon - Versione Rossa",
        "(Italy) (SGB)",
        "DMG-APAI",
        692432107,
        1048576,
        "It",
        "Italy",
        "/pokemon_red64.sprite"
    },
    {
        "50-41665",
        "Pocket Monsters - Aka",
        "(Japan) (SGB)",
        "DMG-APAJ",
        325396229,
        524288,
        "Ja",
        "Japan",
        "/pokemon_red64.sprite"
    },
    {
        "49-47206",
        "Pocket Monsters - Aka",
        "(Japan) (Rev 1) (SGB)",
        "DMG-APAJ",
        3078349280,
        524288,
        "Ja",
        "Japan",
        "/pokemon_red64.sprite"
    },
    {
        "24-14410",
        "Pokemon - Edicion Roja",
        "(Spain) (SGB)",
        "DMG-APAS",
        3629153674,
        1048576,
        "Es",
        "Spain",
        "/pokemon_red64.sprite"
    },
    {
        "156-56789",
        "Pocket Monsters - Midori",
        "(Japan) (SGB)",
        "DMG-APBJ",
        3135950123,
        524288,
        "Ja",
        "Japan",
        "/pokemon_midori64.sprite"
    },
    {
        "155-62791",
        "Pocket Monsters - Midori",
        "(Japan) (Rev 1) (SGB)",
        "DMG-APBJ",
        934186436,
        524288,
        "Ja",
        "Japan",
        "/pokemon_midori64.sprite"
    },
    {
        "18-10825",
        "Mario's Picross",
        "(USA, Europe) (SGB)",
        "DMG-APCE",
        4074132141,
        262144,
        "En",
        "USA, Europe",
        "/picross64.sprite"
    },
    {
        "19-42730",
        "Mario no Picross",
        "(Japan) (SGB)",
        "DMG-APCJ",
        391329536,
        262144,
        "Ja",
        "Japan",
        "/picross64.sprite"
    },
    {
        "93-13923",
        "Pinball Deluxe",
        "(Europe)",
        "DMG-APDP",
        42355762,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "f0de49dcb6f8e16b4c7bfab7bf36df5b807e1f8d",
        "Kwirk - He's A-maze-ing!",
        "(USA, Europe)",
        "DMG-APE",
        566856434,
        32768,
        "En",
        "USA, Europe"
   , " " },
    {
        "203-11964",
        "Pokemon - Blaue Edition",
        "(Germany) (SGB)",
        "DMG-APED",
        2620613383,
        1048576,
        "De",
        "Germany",
        "/pokemon_blue64.sprite"
    },
    {
        "211-40202",
        "Pokemon - Blue Version",
        "(USA, Europe) (SGB)",
        "DMG-APEE",
        3604646426,
        1048576,
        "En",
        "USA, Europe",
        "/pokemon_blue64.sprite"
    },
    {
        "211-40196",
        "Pokemon - Blue Version Rev ?",
        "(USA, Europe) (SGB)",
        "DMG-APEE",
        3604646426,
        1048576,
        "En",
        "USA, Europe",
        "/pokemon_blue64.sprite"
    },
    {
        "203-22180",
        "Pokemon - Version Bleue",
        "(France) (SGB)",
        "DMG-APEF",
        1357052957,
        1048576,
        "Fr",
        "France",
        "/pokemon_blue64.sprite"
    },
    {
        "203-24220",
        "Pokemon - Versione Blu",
        "(Italy) (SGB)",
        "DMG-APEI",
        1292469417,
        1048576,
        "It",
        "Italy",
        "/pokemon_blue64.sprite"
    },
    {
        "229-56374",
        "Pocket Monsters - Ao",
        "(Japan) (SGB)",
        "DMG-APEJ",
        3829828884,
        524288,
        "Ja",
        "Japan",
        "/pokemon_blue64.sprite"
    },
    {
        "203-5335",
        "Pokemon - Edicion Azul",
        "(Spain) (SGB)",
        "DMG-APES",
        3646166777,
        1048576,
        "Es",
        "Spain",
        "/pokemon_blue64.sprite"
    },
    {
        "99fcfd3279a0ca6dd3d5c1d3eb7be9787a797d48",
        "Pinball Fantasies",
        "(USA, Europe)",
        "DMG-APFE",
        4032211217,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "398becff71f79abe40361ab7604faa54d18e85b9",
        "Pachi-Slot Hisshou Guide GB",
        "(Japan) (SGB)",
        "DMG-APGJ",
        1860237241,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "9b373377b8d23c82733dcbbe720e942387dc6686",
        "Totsugeki! Papparatai",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-APHJ",
        183167227,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "08c03b20a4c548a5a9b49426b70f33f43d9bcb29",
        "Pinball Mania",
        "(Europe)",
        "DMG-APIP",
        3989360930,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "1eeb03bd7d965c5d13d053e7aa5dbf00287e128f",
        "Puzzle Boy",
        "(Japan)",
        "DMG-APJ",
        719509276,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "1dbdcc38dad400613f287d14d9e0d545464e10c3",
        "Pocket Cooking",
        "(Japan)",
        "CGB-APJJ",
        4121646413,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "20f9891e265e562743901e8776562cb8a8b75b87",
        "Pachinko Monogatari Gaiden",
        "(Japan) (SGB)",
        "DMG-APKJ",
        1387116537,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "e3a2e5da026849dd2d86ca7fabbe13eff1a22563",
        "Pocket Love",
        "(Japan) (SGB)",
        "DMG-APLJ",
        3580930527,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "1b6c1585501a14510f334b75ac0c1c2e01b1fe7f",
        "Pagemaster, The",
        "(USA) (SGB)",
        "DMG-APME",
        3455742437,
        262144,
        "En",
        "USA"
   , " " },
    {
        "19cea50d713d5a43519351b73449e9cd8a0a6921",
        "Pagemaster, The",
        "(Europe) (SGB)",
        "DMG-APMP",
        3384710867,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "0196de271d1ed7b5bafc0f7b4edd205975e0d091",
        "Puzzle Nintama Rantarou GB",
        "(Japan) (SGB)",
        "DMG-APNJ",
        2502500538,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "3bcfd579f80a3a4865a878eca7b0b9f82ec4f373",
        "Pocket Bomberman",
        "(Japan) (SGB)",
        "DMG-APOJ",
        556484517,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "4f649205e6e467cc6716e36dc97349cb2dfc789a",
        "Purikura Pocket - Fukanzen Joshikousei Manual",
        "(Japan) (SGB)",
        "DMG-APPJ",
        2898627068,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "d6c6da6ff449400e2f34346623285619cf8b4f78",
        "Purikura Pocket - Fukanzen Joshikousei Manual",
        "(Japan) (Rev 1) (NP)",
        "DMG-APPJ",
        1474937213,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "d33999228912612cfcfd746b5a7e67ebe1c90f5e",
        "Power Quest",
        "(USA) (En-US,En-GB,Fr,De,Es,It) (SGB) (GB Comp.)",
        "DMG-APQE",
        3671520948,
        1048576,
        "En-US,En-GB,Fr,De,Es,It",
        "USA"
   , " " },
    {
        "49d102cfb35187a44b76c80efcbb1bfa615bbd78",
        "Power Quest",
        "(Europe) (En-US,En-GB,Fr,De,Es,It) (SGB) (GB Comp.)",
        "DMG-APQP",
        820110695,
        1048576,
        "En-US,En-GB,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "fff06a2163a14ff7ec022aa46ba46b180cefa7a7",
        "PGA European Tour",
        "(USA, Europe) (SGB)",
        "DMG-APRE",
        2105405284,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "97-26363",
        "Pokemon - Gelbe Edition\n - Special Pikachu Edition",
        "(Germany) (CGB+SGB Enhanced)",
        "DMG-APSD",
        2046944346,
        1048576,
        "De",
        "Germany",
        "/pokemon_yellow64.sprite" 
    },
    {
        "151-1148",
        "Pokemon - Yellow Version\n - Special Pikachu Edition",
        "(USA, Europe) (CGB+SGB Enhanced)",
        "DMG-APSE",
        2102558050,
        1048576,
        "En",
        "USA, Europe",
        "/pokemon_yellow64.sprite" 
    },
    {
        "95-47041",
        "Pokemon - Version Jaune\n - Edition Speciale Pikachu",
        "(France) (CGB+SGB Enhanced)",
        "DMG-APSF",
        3493078761,
        1048576,
        "Fr",
        "France",
        "/pokemon_yellow64.sprite" 
    },
    {
        "92-20111",
        "Pokemon - Versione Gialla\n - Speciale Edizione Pikachu",
        "(Italy) (CGB+SGB Enhanced)",
        "DMG-APSI",
        2337734195,
        1048576,
        "It",
        "Italy",
        "/pokemon_yellow64.sprite" 
    },
    {
        "30-60889",
        "Pocket Monsters - Pikachu",
        "(Japan) (Rev 2) (SGB)",
        "DMG-APSJ",
        4248676351,
        1048576,
        "Ja",
        "Japan",
        "/pokemon_yellow64.sprite" 
    },
    {
        "32-39977",
        "Pocket Monsters - Pikachu",
        "(Japan) (SGB)",
        "DMG-APSJ",
        1321751812,
        1048576,
        "Ja",
        "Japan",
        "/pokemon_yellow64.sprite" 
    },
    {
        "31-34904",
        "Pocket Monsters - Pikachu",
        "(Japan) (Rev 1) (SGB)",
        "DMG-APSJ",
        2723437875,
        1048576,
        "Ja",
        "Japan",
        "/pokemon_yellow64.sprite" 
    },
    {
        "29-55684",
        "Pocket Monsters - Pikachu",
        "(Japan) (Rev 3) (SGB)",
        "DMG-APSJ",
        3924183098,
        1048576,
        "Ja",
        "Japan",
        "/pokemon_yellow64.sprite" 
    },
    {
        "82-22071",
        "Pokemon - Edicion Amarilla\n - Edicion Especial Pikachu",
        "(Spain) (CGB+SGB Enhanced)",
        "DMG-APSS",
        2521528848,
        1048576,
        "Es",
        "Spain",
        "/pokemon_yellow64.sprite" 
    },
    {
        "f2fe2f79feb1225afe5e771725a655a7ec618f7b",
        "Pac-In-Time",
        "(USA) (SGB)",
        "DMG-APTE",
        1352752584,
        262144,
        "En",
        "USA"
   , " " },
    {
        "64ca9a117aa533195b6494d88f4d9a05260e7c52",
        "Pac-In-Time",
        "(Japan) (SGB)",
        "DMG-APTJ",
        442120410,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "a17d6c41e3aae676391eb16853db7b0f6d0abc07",
        "Pac-In-Time",
        "(Europe) (Rev 1) (SGB)",
        "DMG-APTP",
        2146934265,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "ffba304a37fae7bb71a354177805ba48c76e0d2f",
        "Pac-In-Time",
        "(Europe) (SGB)",
        "DMG-APTP",
        2257630865,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "9ac7d874084af25cfa59eaca0a3dc28e3751a883",
        "Pocket GI Stable",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-APUJ",
        3290728270,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "70afd8eae8262dcbc8ee76da8967b87aa172b22a",
        "Pocket Love 2",
        "(Japan) (SGB)",
        "DMG-APVJ",
        2676244661,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "114f2db68b880cec8a055810b340eb7242bdfd2a",
        "Power Pro GB",
        "(Japan) (SGB)",
        "DMG-APWJ",
        772947803,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "e40ceb309ea0ca22eb67dc0fd308a865da4a9fb7",
        "Power Pro GB",
        "(Japan) (Rev 1) (SGB)",
        "DMG-APWJ",
        1908320341,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "4194a5e7589f5f31faf59c0adf0104284b5b22bc",
        "Bug's Life, A",
        "(USA) (SGB) (GB Comp.)",
        "DMG-APXE",
        2204107898,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "dc2e556cbf985c9bd3bf17324899c37fdbba9bd6",
        "Bug's Life, A",
        "(Europe) (SGB) (GB Comp.)",
        "DMG-APXP",
        3683901686,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "95e900bcd97f5165a07d65c1965f1f5a21225ff3",
        "Pocket Puyo Puyo Tsuu",
        "(Japan) (SGB)",
        "DMG-APYJ",
        1139047041,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "f000cd00a961e0abbe6027343265d9746622f79e",
        "Pocket Puyo Puyo Tsuu",
        "(Japan) (Rev 1) (SGB) (NP)",
        "DMG-APYJ",
        3404243735,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "1343b7d43e42a973218562986a1cab2d597af505",
        "Gekitou Power Modeller",
        "(Japan) (SGB)",
        "DMG-APZJ",
        1220668962,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "1283b1ada68504bc788cdcad8c9b4b6ddff6aa7d",
        "Goukaku Boy Series - 99 Nendo Ban Eitango Center 1500",
        "(Japan)",
        "DMG-AQ2J",
        2618102979,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "185-43582",
        "4x4 World Trophy",
        "(Europe) (En,Fr,De,Es,It) (GB Comp.)",
        "DMG-AQ3P",
        4130002782,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "146-57765",
        "1942",
        "(USA, Europe)",
        "CGB-AQ4E",
        2269320818,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "3cadfc903a152becdbb7cb4999643d48b5286cce",
        "NFL Quarterback Club II",
        "(USA, Europe)",
        "DMG-AQ9E",
        116409200,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "0a558513393fb01c4c2980a51aaa1f8832c1a8fb",
        "NFL Quarterback Club '95",
        "(Japan)",
        "DMG-AQ9J",
        4006309948,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "be053b7b20a06099d703f6d3ba0ec8e16b79298b",
        "NFL Quarterback Club 96",
        "(USA, Europe)",
        "DMG-AQBE",
        3582180430,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "7461c423d3daef9c63971291dbeccc250e264b71",
        "Ms. Pac-Man - Special Color Edition",
        "(USA) (SGB) (GB Comp.)",
        "DMG-AQCE",
        272507181,
        524288,
        "En",
        "USA"
   , " " },
    {
        "18219ab40bcc3a186ee4ba5fe4feac84cb2b032a",
        "Ms. Pac-Man - Special Colour Edition",
        "(Europe) (SGB) (GB Comp.)",
        "DMG-AQCP",
        4063452585,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "d013a4d4391d34d7dd0e354abb4c2c3ca4c39d51",
        "Pro Mahjong Tsuwamono GB",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AQDJ",
        341956824,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "583ff40bf56e7957dc1e1eb0456a8269e5701048",
        "Kachiuma Yosou Keiba Kizoku EX '95",
        "(Japan)",
        "DMG-AQEJ",
        2624211224,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "9401014e5518ce3df64407ccbab270e24d990f8d",
        "Shaq Fu",
        "(USA) (SGB)",
        "DMG-AQFE",
        2127839206,
        524288,
        "En",
        "USA"
   , " " },
    {
        "a56e516ec247937befea9e3743f265715e00880b",
        "Shin Keiba Kizoku Pocket Jockey",
        "(Japan) (SGB)",
        "DMG-AQGJ",
        1225755718,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "3622f0589a5716057af5d646f7d4612fc38d7733",
        "Toy Story",
        "(USA) (SGB)",
        "DMG-AQHE",
        3544352716,
        524288,
        "En",
        "USA"
   , " " },
    {
        "ca90590e693e9229c547cb4ac0f17f022f8392fe",
        "Toy Story",
        "(USA) (Rev 1) (SGB)",
        "DMG-AQHE",
        1865856749,
        524288,
        "En",
        "USA"
   , " " },
    {
        "e4c9068094c34a2b6225c28236d2b2f19ef300a1",
        "Toy Story",
        "(Europe) (SGB)",
        "DMG-AQHP",
        3638887951,
        524288,
        "En",
        "Europe"
   , " " },
    {
        "a6a07c9fcd6ee40c4708b79588c576cc7f3f4b16",
        "Pocket Lure Boy",
        "(Japan)",
        "CGB-AQIJ",
        899847720,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "fc4958af1c28b6d5619b52bb0648bb35882e4de3",
        "Bishoujo Senshi Sailor Moon",
        "(Japan)",
        "DMG-AQJ",
        3262529139,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "4eca827cfa8f3f9861119897ee2617fbb15b7d3d",
        "Jack no Daibouken - Daimaou no Gyakushuu",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AQJJ",
        3488952922,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "b65f9f495242c6ee6af5ecd2dde92d9eaf8d4015",
        "Bass Masters Classic",
        "(USA, Europe) (GB Comp.)",
        "DMG-AQME",
        3070240996,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "6cd2387e3192d6b98c75510269bc9d1ff1103ea6",
        "Kidou Senkan Nadesico - Ruri Ruri Mahjong",
        "(Japan)",
        "CGB-AQNJ",
        1680330106,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "937fbaf5ee51afd470008b7dc547391f13e3ab4e",
        "Ready 2 Rumble Boxing",
        "(Europe)",
        "CGB-AQOP",
        3198381049,
        2097152,
        "En",
        "Europe"
   , " " },
    {
        "d72b972b99e10e0bbd28954504b3ad513259302c",
        "Kanzume Monsters Parfait",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AQPJ",
        1834703296,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "d914ce0da1313d85c34c5c5c2d91114c745b6340",
        "Quest - Brian's Journey",
        "(USA) (GB Comp.)",
        "DMG-AQRE",
        2596435525,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "b27ca6255226928689c55f61dae272dc4cf587e4",
        "Qix Adventure",
        "(Japan)",
        "CGB-AQSJ",
        1523492009,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "25dc8bfd880272e6183be97693fadac12bee2d06",
        "Quest - Fantasy Challenge",
        "(USA) (SGB) (GB Comp.)",
        "DMG-AQTE",
        2552780661,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "c295ce058409c40bc16116ffcc6a1fd7fb9877b2",
        "Holy Magic Century",
        "(Europe) (En,Fr,De) (SGB) (GB Comp.)",
        "DMG-AQTP",
        2883112622,
        1048576,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "22352e21c4684dfb6a2b0ee92bec70c193f5a2ff",
        "Qui Qui",
        "(Japan) (GB Comp.)",
        "DMG-AQUJ",
        1456958370,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "c05c90bfce52a661743b66d33850ad63227b6e75",
        "Bomberman Quest",
        "(Europe) (En,Fr,De) (SGB) (GB Comp.)",
        "DMG-AQVP",
        1520147174,
        2097152,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "91ad9f9bc363a86bba34fb1893996a888e518b97",
        "Qix Adventure",
        "(Europe)",
        "CGB-AQYP",
        3609844544,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "e658cfae3bd3ce71b3e8123a7c6e260b2212815d",
        "Hayaoshi Quiz - Ouza Ketteisen",
        "(Japan) (SGB)",
        "DMG-AQZJ",
        1618115577,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "c1a0f238133ca91fa442ed3e8db13424af4aa1d7",
        "Magic Knight Rayearth 2nd. - The Missing Colors",
        "(Japan) (SGB)",
        "DMG-AR2J",
        2108922,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "44d21a1560a48beb506918f3527eabed68ac45e7",
        "Goukaku Boy Series - Gakken - Rekishi 512",
        "(Japan)",
        "DMG-AR3J",
        280855482,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "763e37a4e77fbb1274dfbe97bc4cffe0c144cb3e",
        "Caterpillar Construction Zone",
        "(USA, Europe) (GB Comp.)",
        "DMG-AR4E",
        3524118128,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "f0f91a5eb2c825a3c38e416b74b94bda3f32e598",
        "Rampart",
        "(USA)",
        "CGB-AR5E",
        3585010990,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "a4626213ae2dceedfdb69ee0bbfc2746bec4d50f",
        "Tom Clancy's Rainbow Six",
        "(USA, Europe) (En,Fr,De)",
        "CGB-AR6E",
        3878626947,
        1048576,
        "En,Fr,De",
        "USA, Europe"
   , " " },
    {
        "21172395bf500f551aab8194824755ef2a5f86b6",
        "Turok - Rage Wars",
        "(USA, Europe) (En,Fr,De,Es)",
        "CGB-AR7E",
        2020301492,
        1048576,
        "En,Fr,De,Es",
        "USA, Europe"
   , " " },
    {
        "c6813043289f29b13f1ebc494cccc3e673f5223f",
        "Primal Rage",
        "(USA, Europe)",
        "DMG-AR9E",
        3378352763,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "b1a9c7617a333aa65e5018a1350e175b28077463",
        "Astro Rabby",
        "(Japan)",
        "DMG-ARA",
        1642368751,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "cac084d5dd33e85e880bc94087a3e202a57328cb",
        "Lodoss-tou Senki - Eiyuu Kishiden GB",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-ARAJ",
        1726047931,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "a533132e74ba03c58053516ca6a2eb7d6a477bef",
        "Nettou Real Bout Garou Densetsu Special",
        "(Japan) (SGB)",
        "DMG-ARBJ",
        4093844812,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "13e3f4bdbdd3f7ed0cc58918fda1b8aafb49a52b",
        "Luca no Puzzle de Daibouken!",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-ARCJ",
        988543704,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "57c1e5c0b22c86923c40ca7438d7b1c6d00e847a",
        "Race Days",
        "(USA)",
        "DMG-ARDE",
        1143218355,
        262144,
        "En",
        "USA"
   , " " },
    {
        "58fc3c85df2db9b31b96ae77ada28ae2497ad898",
        "Race Days",
        "(Europe)",
        "DMG-ARDP",
        3115012889,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "da68c26907dc43dcab35bf5ba74027e80630184a",
        "Sakata Gorou Kudan no Renju Kyoushitsu",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AREJ",
        3040947311,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "5cddd238cb1ff981d5bf024292d595d971c4d0c3",
        "Rugrats Movie, The",
        "(USA) (SGB)",
        "DMG-ARGE",
        306842886,
        524288,
        "En",
        "USA"
   , " " },
    {
        "4852838f4717c1a99fd5008f06ec9a2484d626a4",
        "Resident Evil Gaiden",
        "(USA)",
        "CGB-ARHE",
        4173660699,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "968e7562351911d105aa171493b651c5483ef044",
        "Prince of Persia",
        "(USA, Europe) (En,Fr,De,Es,It) (GB Comp.)",
        "DMG-ARIE",
        3871262417,
        1048576,
        "En,Fr,De,Es,It",
        "USA, Europe"
   , " " },
    {
        "22359c7ad9cbe7caacc0fc0224f187e2948defe2",
        "Super Star Wars - Return of the Jedi",
        "(USA, Europe) (SGB)",
        "DMG-ARJE",
        1823948231,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "06a81b9dfd76fde5118a688ac788ba3330666ab5",
        "Legend of the River King GB",
        "(Germany) (SGB) (GB Comp.)",
        "DMG-ARKD",
        306312181,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "360f41c8106ae73e1b55415bf293042ef4dd6782",
        "Legend of the River King GBC",
        "(USA) (SGB) (GB Comp.)",
        "DMG-ARKE",
        2122456903,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "f9131b7c074c7d81466a27744fd3ce4de07c0c06",
        "Legend of the River King GB",
        "(Europe) (SGB) (GB Comp.)",
        "DMG-ARKP",
        2280625456,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "ef9cd05904156702f2e0db34f365e8416e0004fe",
        "International Rally",
        "(USA) (SGB) (GB Comp.)",
        "DMG-ARLE",
        3093288786,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "ee55d96fdb389fd00151964ac49b8784687b22eb",
        "It's a World Rally",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-ARLJ",
        971195984,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "01a56512d8b8400c9a581a05cd67e8fe5715581c",
        "Cross Country Racing",
        "(Europe) (En,Fr,De) (SGB) (GB Comp.)",
        "DMG-ARLP",
        3082362857,
        1048576,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "3a722d491581d691266c69654ec83640705639cd",
        "Kaseki Sousei Reborn II - Monster Digger",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-ARMJ",
        3212888215,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "f2a0260f1a3f705c90986a2e4f821ad723e2f8aa",
        "Mickey's Racing Adventure",
        "(USA, Europe) (En,Fr,De,Es,It)",
        "CGB-ARNE",
        3588769268,
        4194304,
        "En,Fr,De,Es,It",
        "USA, Europe"
   , " " },
    {
        "1f64dc547f770c71bb4cdd783b76a4c1fe7386b2",
        "Rentaiou",
        "(Japan)",
        "DMG-AROJ",
        592482859,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "06468ae7843cd97040e3ecffbd8084e914146479",
        "Rampage - World Tour",
        "(USA, Europe) (GB Comp.)",
        "DMG-ARPE",
        2955477375,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "4e78946ab0200420523bdc724951f5eea2a75870",
        "Rugrats Movie, The",
        "(USA) (SGB) (GB Comp.)",
        "DMG-ARQE",
        4273874438,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "f2fc2406a0f0487b6eac4c04bced661f2c4c0422",
        "Roadsters",
        "(USA) (En,Fr,De,Es,It,Nl) (GB Comp.)",
        "DMG-ARRE",
        951067326,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "USA"
   , " " },
    {
        "f48888727ca2ebf96c387365e8e286782629d145",
        "Roadsters",
        "(Europe) (En,Fr,De,Es,It,Nl) (GB Comp.)",
        "DMG-ARRP",
        1903899941,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "db74cf7d3cf72b1a431f32ac1dadf1ca1e4df47c",
        "Reservoir Rat",
        "(Europe) (En,Fr,De,Es,It)",
        "DMG-ARSP",
        758374773,
        262144,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "225fb91fb15ad8e70a24c5bef07305403a621d5a",
        "Robot Ponkots - Sun Version - Comic Bom Bom Special Version",
        "(Japan) (SGB)",
        "DMG-ARTJ",
        2239447065,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "ed22661c4bdf754c1251a05953ad9fd6263c0d8c",
        "R-Type DX",
        "(Japan) (En) (GB Comp.)",
        "DMG-ARUJ",
        908323766,
        1048576,
        "En",
        "Japan"
   , " " },
    {
        "f86f6bdd68791a2e44bd5cfa64ce1433d48d796c",
        "Grander Musashi RV",
        "(Japan) (SGB)",
        "DMG-ARVJ",
        3917426330,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "b094d83a368f6ceefda311b5ed800c6bac1058f2",
        "Rox",
        "(USA, Europe) (GB Comp.)",
        "DMG-ARXE",
        781469557,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "4ff142cadd7efdf7fd5ee124f0841ed98377b235",
        "Rox",
        "(Japan) (En) (GB Comp.)",
        "DMG-ARXJ",
        1272397209,
        262144,
        "En",
        "Japan"
   , " " },
    {
        "146a01c7d432a89fc2ead84d33b05787e608b625",
        "Magic Knight Rayearth",
        "(Japan) (SGB)",
        "DMG-ARYJ",
        2491648641,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "44beb161573c8d2d9f9b8790dad60e95c305e847",
        "Rugrats Movie, The",
        "(Europe) (SGB) (GB Comp.)",
        "DMG-ARZP",
        3020494336,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "a1a6632002f30add660fe9fd48c7e682df57075e",
        "From TV Animation Slam Dunk 2 - Zenkoku e no Tip Off",
        "(Japan) (SGB)",
        "DMG-AS2J",
        3409285130,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "cb4ede3b5a9efdf44116a4bb21c8287efc2a9b63",
        "Goukaku Boy Series - Koukou Nyuushi Derujun - Rika Anki Point 250",
        "(Japan)",
        "DMG-AS3J",
        1883670357,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "743998b4ddb4c6e5fe25c5b9e38245c4991d2e20",
        "Same Game",
        "(Japan) (SGB)",
        "DMG-AS5J",
        1882799119,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "7d34d42f978d49abd5263a3b83fb0605c9fd5ce7",
        "Spy vs Spy",
        "(USA)",
        "CGB-AS6E",
        4031135057,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "80c0f470a9476dbd1727a887b702a7db5a118fa3",
        "Spy vs Spy",
        "(Japan) (Rev 1) (NP)",
        "CGB-AS6J",
        3947353659,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "abc0cb0db602f5a5ee3356bd66ce281c462b0c88",
        "Spy vs Spy",
        "(Japan)",
        "CGB-AS6J",
        242757911,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "b3d1a2e4338847f457e65d4cb817000ec1ffdbbb",
        "Spy vs Spy",
        "(Europe) (En,Fr,De,Es,It,Nl,Sv)",
        "CGB-AS6P",
        1899785239,
        1048576,
        "En,Fr,De,Es,It,Nl,Sv",
        "Europe"
   , " " },
    {
        "ad9f7cc8f85221332384635755e5e797b0eff327",
        "Tokimeki Memorial Pocket - Sport Hen - Koutei no Photograph",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AS7J",
        2028031913,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "1e8a085bff6a8ce4fc2b54372806368c6f80c2cc",
        "Shougi 2",
        "(Japan) (GB Comp.)",
        "DMG-AS8J",
        2809433387,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "692000c943210f83e7ea291f67fdad682c7ea933",
        "Sangokushi - Game Boy Ban 2",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AS9J",
        3884430412,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "42e46aac72e1c35b71d26de74c8758988284cfc7",
        "Pocket Kanjirou",
        "(Japan) (SGB)",
        "DMG-ASAJ",
        3783978415,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "aa68f89de54e8fb6b9caba6f33d27680e7c81c12",
        "Tetris Blast",
        "(USA, Europe) (SGB)",
        "DMG-ASBE",
        2078648127,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "91d4477ee483f5493230463076057d4b4c2a5418",
        "Super Bombliss",
        "(Japan) (SGB)",
        "DMG-ASBJ",
        3036938157,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "2cbb79821bd8cac1e3ea0218958eac2f63595c83",
        "Sports Collection",
        "(Japan)",
        "DMG-ASCJ",
        618465760,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "dee22ed79ae53bce9f1b77f2dd5e1951090018fc",
        "Boomer's Adventure in ASMIK World",
        "(USA)",
        "DMG-ASE",
        274448832,
        65536,
        "En",
        "USA"
   , " " },
    {
        "b87e6d1e8cc59015a92daba3c3bff6d8b8c7cab8",
        "Shanghai Pocket",
        "(Japan) (SGB)",
        "DMG-ASEJ",
        1804533548,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "7f353b7eacc45e7198e04baa1b9496f81e4074a3",
        "Street Fighter II",
        "(USA, Europe) (Rev 1) (SGB)",
        "DMG-ASFE",
        1419815587,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "db4113bd542df6e90a49631f431053aa187dff94",
        "Street Fighter II",
        "(USA, Europe) (SGB)",
        "DMG-ASFE",
        187269898,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "b926237dab61b09f412db4bd42bee5343bf6d454",
        "Street Fighter II",
        "(Japan) (SGB)",
        "DMG-ASFJ",
        3346396296,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "5aec765a6aaca2689693e9ca3b94c587aa81d6c9",
        "Shinri Game 2, The - Oosaka Hen",
        "(Japan)",
        "DMG-ASGJ",
        1191793156,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "3c7d9a6775759056d818959874fa62ccdacf62f3",
        "Aoki Densetsu Shoot!",
        "(Japan) (SGB)",
        "DMG-ASHJ",
        141719392,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "e83dec77748910c97b667291b06cdb208fedc51d",
        "Small Soldiers",
        "(USA, Europe) (SGB)",
        "DMG-ASIE",
        3690006225,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "fb441a09c1e5954e26d065f6eb5caa8c4aa4b221",
        "Teke Teke! Asmik-kun World",
        "(Japan)",
        "DMG-ASJ",
        2820097293,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "44b635e215065a36927b09724e4dd254a85437fd",
        "Nihon Daihyou Team France de Ganbare! - J.League Supporter Soccer",
        "(Japan) (SGB)",
        "DMG-ASJJ",
        2447818612,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "be4db38beab76b1ccf38dfebb590e20ff2d7b60c",
        "Nihon Daihyou Team - Eikou no Eleven",
        "(Japan) (SGB)",
        "DMG-ASKJ",
        3835135022,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "b50a1c7a7e9a2ff0fd57f9f8881bff36dbd67ae5",
        "Selection I & II - Erabareshi Mono & Ankoku no Fuuin",
        "(Japan) (SGB)",
        "DMG-ASLJ",
        2785179929,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "d5a42676dc71a86b4980b14576655d7aeed28a8d",
        "Superman",
        "(USA, Europe) (SGB)",
        "DMG-ASME",
        898498705,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "054d708e9efe0b1423b2247be406627932c40551",
        "Smurfs Nightmare, The",
        "(USA)",
        "CGB-ASNE",
        3037507556,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "cb1e4fc75c9a4e1d99b88b9fc761c38c2b8783bd",
        "Smurfs Nightmare, The",
        "(Europe) (En,Fr,De,Es)",
        "CGB-ASNP",
        1872212035,
        1048576,
        "En,Fr,De,Es",
        "Europe"
   , " " },
    {
        "af4f78e1bf7f9b644229020b0d4205eccb5a3f7a",
        "seaQuest DSV",
        "(USA, Europe) (SGB)",
        "DMG-ASQE",
        832814489,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "a0da1312d3aed6c6e7f97d4929e27688472bec1c",
        "Street Racer",
        "(USA, Europe)",
        "DMG-ASRE",
        4244327652,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "307428b09feb781e9c4660cfad46c4562945028b",
        "Street Racer",
        "(Japan)",
        "DMG-ASRJ",
        1538968232,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "c61671515877bea07ea0dd4048794216cab0f3ed",
        "Star Trek Generations - Beyond the Nexus",
        "(USA) (SGB)",
        "DMG-ASTE",
        2670531014,
        131072,
        "En",
        "USA"
   , " " },
    {
        "e12d894ed8e21f92bf76679f4ba2c4cb9882e774",
        "Star Trek Generations - Beyond the Nexus",
        "(Europe) (SGB)",
        "DMG-ASTP",
        3643134301,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "a058d901c60792d7f4cf97915a913f0d66569514",
        "Shutokou Racing, The",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-ASUJ",
        921141709,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "6c948c8de429376677326da04fe5af7fc23b76dc",
        "Shadowgate Classic",
        "(USA, Europe) (En,Fr,De,Es,Sv) (GB Comp.)",
        "DMG-ASWE",
        4138235557,
        1048576,
        "En,Fr,De,Es,Sv",
        "USA, Europe"
   , " " },
    {
        "1b3e8a18afc89086e8d02353f26e93fed8130f98",
        "Shadowgate Return",
        "(Japan) (GB Comp.)",
        "DMG-ASWJ",
        466451824,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "2b674283b140ac75f6a3194fc51567572b8ab236",
        "Shadowgate Classic",
        "(Europe) (En,Fr,De,Es,Sv) (Rev 1) (GB Comp.)",
        "DMG-ASWP",
        3543659600,
        1048576,
        "En,Fr,De,Es,Sv",
        "Europe"
   , " " },
    {
        "fd7cf12dd9959ae2c18583ff757ed394d26dd3cb",
        "San Francisco Rush 2049",
        "(USA, Europe)",
        "CGB-ASXE",
        4013330198,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "6f3414d25dc477e7e9a42beabf998c56026154f6",
        "Bakuchou Retsuden Shou - Hyper Fishing",
        "(Japan) (SGB)",
        "DMG-ASYJ",
        2872342468,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "d0327929be45658512ff892a0b27d8bf5924a43d",
        "Nettou Samurai Spirits - Zankurou Musouken",
        "(Japan) (SGB)",
        "DMG-ASZJ",
        2573011000,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "406df706b8258a652308c2ac4f0d035d602e9909",
        "Goukaku Boy Series - Eitango Target 1900",
        "(Japan)",
        "DMG-AT2J",
        4164969824,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "c7b75b4be128cae81f8f274c71d7db25b8d34227",
        "Game de Hakken!! Tamagotchi 2",
        "(Japan) (SGB)",
        "DMG-AT3J",
        428383299,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "1205976003918b514014f5f32aee4fef1c06a238",
        "Tintin in Tibet",
        "(Europe) (En,Fr,De,Nl) (SGB)",
        "DMG-AT6P",
        1563154296,
        262144,
        "En,Fr,De,Nl",
        "Europe"
   , " " },
    {
        "e4d350e08392edf6e063024db3f65213bc9e435e",
        "Tintin in Tibet",
        "(Europe) (En,Es,It,Sv) (SGB)",
        "DMG-AT6X",
        2055045647,
        262144,
        "En,Es,It,Sv",
        "Europe"
   , " " },
    {
        "75fbccf688d9d6d665a40ff9aabced66b3f411d3",
        "Magical Tetris Challenge",
        "(USA)",
        "CGB-AT7E",
        4114413164,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "b602cd75236b01102cf1a10079ebd4570eaf233e",
        "Tetris Adventure - Susume Mickey to Nakama-tachi",
        "(Japan) (Rev 1)",
        "CGB-AT7J",
        3082847007,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "b94f896072d4802257424cd0b7cdae68d9738be0",
        "Tetris Adventure - Susume Mickey to Nakama-tachi",
        "(Japan)",
        "CGB-AT7J",
        3942049010,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "230c6987dd3ed8911bda0dbba4ead13a0fdce538",
        "Micro Machines 1 and 2 - Twin Turbo",
        "(USA, Europe)",
        "CGB-AT8E",
        1574123499,
        2097152,
        "En",
        "USA, Europe"
   , " " },
    {
        "1d8eff915f0e863fc8e0457b0bfcd3afa09ed6e2",
        "Tomb Raider",
        "(USA, Europe) (En,Fr,De,Es,It)",
        "CGB-AT9E",
        696843384,
        4194304,
        "En,Fr,De,Es,It",
        "USA, Europe"
   , " " },
    {
        "ebef12f7b2d1fb91ab7bd191044e456693c6324e",
        "Tamagotchi",
        "(USA, Europe) (SGB)",
        "DMG-ATAE",
        3690052896,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "27a381e9cec299027b72b13287e5faa8376c9629",
        "Tamagotchi",
        "(France) (SGB)",
        "DMG-ATAF",
        139207681,
        524288,
        "Fr",
        "France"
   , " " },
    {
        "5f937f19bb139bbecfbea43699d86656e53da442",
        "Turok - Battle of the Bionosaurs",
        "(Japan)",
        "DMG-ATBJ",
        3364779293,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "dd2e1aac3fe09f40fc37ce44e4e227a98bbde00c",
        "Trump Collection GB",
        "(Japan)",
        "DMG-ATCJ",
        2202123791,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "cbdaca2e03c794c3a383d89b9791ee4a9c532335",
        "Battle Arena Toshinden",
        "(USA) (SGB)",
        "DMG-ATDE",
        755765363,
        524288,
        "En",
        "USA"
   , " " },
    {
        "613b58a58009e4c7bd81cd9be827b63dbb3c6256",
        "Nettou Toushinden",
        "(Japan) (SGB)",
        "DMG-ATDJ",
        3722535333,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "9cb43cf23542f462a9da31cb2ca0372442e60344",
        "Nettou Toushinden",
        "(Japan) (Rev 1) (SGB)",
        "DMG-ATDJ",
        3584471346,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "f383accccdace83684c63ffb603215f51eeb3d20",
        "Battle Arena Toshinden",
        "(Europe) (SGB)",
        "DMG-ATDP",
        3459566756,
        524288,
        "En",
        "Europe"
   , " " },
    {
        "f408bf5c5b6410f4b650407ac31d0443bed36bd9",
        "Tetris DX",
        "(World) (SGB) (GB Comp.)",
        "DMG-ATEA",
        1771606354,
        524288,
        "En",
        "World"
   , " " },
    {
        "085c64bd08eb669f36fbaba0046c79f91b970c24",
        "Sanrio Timenet - Mirai Hen",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-ATFJ",
        4024770071,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "51828f49287da8d56d93234c60af58e3631e9eb6",
        "Sanrio Timenet - Mirai Hen",
        "(Japan) (Rev 1) (SGB) (GB Comp.)",
        "DMG-ATFJ",
        396208115,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "8a3767428abd86c8f1903f55f71290a5926a94ea",
        "Tsumego Series 1 - Fujisawa Hideyuki Meiyo Kisei",
        "(Japan) (SGB)",
        "DMG-ATGJ",
        3471731297,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "74e94c374a4c967e1ff75b78ec020156d3477e6f",
        "Tarzan",
        "(Germany)",
        "CGB-ATHD",
        969950593,
        2097152,
        "De",
        "Germany"
   , " " },
    {
        "6ac3788c4761eed28b7d1714f40ec015479a5974",
        "Tarzan",
        "(USA, Europe)",
        "CGB-ATHE",
        1109719344,
        2097152,
        "En",
        "USA, Europe"
   , " " },
    {
        "a4521438526f9569af9f7ad323c374cebb40f1f7",
        "Tarzan",
        "(France)",
        "CGB-ATHF",
        3305353147,
        2097152,
        "Fr",
        "France"
   , " " },
    {
        "588435a5e5e0f1c7fbe1298fdd295c2290f0e621",
        "Tasmanian Devil - Munching Madness",
        "(USA) (En,Fr,De,Es,It) (GB Comp.)",
        "DMG-ATIE",
        907137240,
        1048576,
        "En,Fr,De,Es,It",
        "USA"
   , " " },
    {
        "8f5a7db8ec5162013f001d272e088aff0fc6cf9c",
        "Asmik-kun World 2",
        "(Japan)",
        "DMG-ATJ",
        3400428305,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "4e77db4579fc89636f6ee5750b8d510687832346",
        "Tsumeshougi - Kanki Godan",
        "(Japan) (SGB)",
        "DMG-ATKJ",
        3577957524,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "a796fb5c8dd90596effdb25a708845644cd79c9e",
        "True Lies",
        "(USA, Europe)",
        "DMG-ATLE",
        3991692262,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "601524c2bd424b424c88320dedbb488621399374",
        "Tokoro's Mahjong Jr.",
        "(Japan) (SGB)",
        "DMG-ATMJ",
        1454361164,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "e6ef1fcf38500159104b1b9bd32347a8fe2741a8",
        "Pocket Densha",
        "(Japan) (SGB)",
        "DMG-ATNJ",
        2220602999,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "026efcea900774a6b96a1b2df7c0c1b54f453d69",
        "Othello World",
        "(Japan) (SGB)",
        "DMG-ATOJ",
        1691433581,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "2b2a7e2b45bbc6763528f606ee1e3ba9589ac91a",
        "Sanrio Timenet - Kako Hen",
        "(Japan) (Rev 1) (SGB) (GB Comp.)",
        "DMG-ATPJ",
        1700116288,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "ff47605436604b6b649fc9ee1031fbbe448043b5",
        "Sanrio Timenet - Kako Hen",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-ATPJ",
        1166759837,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "675e754092ccc7db19320007da9e514859513da6",
        "Tonka Raceway",
        "(USA)",
        "CGB-ATQE",
        3154638075,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "a4d3d6217f25d7bf818eb1c5313367eafd3a772a",
        "Tonka Raceway",
        "(Europe)",
        "CGB-ATQP",
        3775443650,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "0c63b5d08d3b2a4ec302cc121bdacdc5509eb9d3",
        "Tetris Plus",
        "(USA, Europe) (SGB)",
        "DMG-ATRE",
        3673963519,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "a31b067e3b5f19ebff283c8b398ce12d3b649290",
        "Tetris Plus",
        "(Japan) (SGB)",
        "DMG-ATRJ",
        784929290,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "db71247123b525d55b47f766f4ae821dae786a75",
        "Tintin - Prisoners of the Sun",
        "(Europe) (En,Fr,De)",
        "DMG-ATSP",
        1442601055,
        262144,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "1e5cc6802028a0ae0a110644ee591c15419ab18a",
        "Bass Fishing Tatsujin Techou",
        "(Japan)",
        "DMG-ATTJ",
        1982195109,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "7a01324fcaf46453e9cdad3a2ee723d00c848a0c",
        "Turok - Battle of the Bionosaurs",
        "(USA, Europe) (En,Fr,De,Es)",
        "DMG-ATUE",
        1368141103,
        262144,
        "En,Fr,De,Es",
        "USA, Europe"
   , " " },
    {
        "9b33d5a733102acf568e5ac6dfceeb464653d6a8",
        "Bakenou TV '94",
        "(Japan) (SGB)",
        "DMG-ATVJ",
        3519279985,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "78dc01aa75f70b9edab3ccc140dcba2e8b3ce80d",
        "Monster Truck Wars",
        "(USA, Europe)",
        "DMG-ATWE",
        409735323,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "43cf4d2ee1b14094a4d9be7102d3c74cb6fd96a8",
        "Tom and Jerry",
        "(USA, Europe)",
        "CGB-ATXE",
        3111914457,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "1d9598971afb6aeb55807405c0a1afee7756214f",
        "Taz-Mania 2",
        "(USA)",
        "DMG-ATZE",
        1288398304,
        131072,
        "En",
        "USA"
   , " " },
    {
        "beeed178a140b9e93583c90e73593c83beb07b62",
        "Smurfs Nightmare, The",
        "(Europe) (En,Fr,De,Es)",
        "DMG-AU3P",
        2398697668,
        262144,
        "En,Fr,De,Es",
        "Europe"
   , " " },
    {
        "b7c178d6fa0b5a0a212dad1d2c20b0a4b7a05488",
        "Sweet Ange",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AUBJ",
        1273606489,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "74fe034e1ba56fe156cd344af5d5f3cf25bed906",
        "Uno 2 - Small World",
        "(Japan) (SGB)",
        "DMG-AUCJ",
        2813158895,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "9fda89331d3a364a0f87a2b8d4738dbd4a3768a5",
        "Bakusou Senki Metal Walker GB - Koutetsu no Yuujou",
        "(Japan) (GB Comp.)",
        "DMG-AUEJ",
        3574909344,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "437d450b33ddfc4504b6ea2fbd7c9959729bc78e",
        "Smurfs Travel the World, The",
        "(Europe) (En,Fr,De,Es)",
        "DMG-AUFP",
        3408687512,
        131072,
        "En,Fr,De,Es",
        "Europe"
   , " " },
    {
        "899a263f7441fd758f2e542014ac595b48653286",
        "Guruguru Garakutas",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AUGJ",
        2125393580,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "8c229a3dec29f72794892dec20cc405f39d50e23",
        "Heroes of Might and Magic",
        "(USA) (En,Fr,De)",
        "CGB-AUHE",
        3137761958,
        1048576,
        "En,Fr,De",
        "USA"
   , " " },
    {
        "f3df3d2626a3809d9443efabd72c4252c48ed333",
        "Heroes of Might and Magic",
        "(Europe) (En,Fr,De)",
        "CGB-AUHP",
        3770285371,
        1048576,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "6aa8538167b14d061ed5d18897e2dc59a91c35aa",
        "America Oudan Ultra Quiz Part 2",
        "(Japan) (Rev 1)",
        "DMG-AUJ",
        1939613436,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "e542bb864e9d79c8c6f72921831827d1637b3601",
        "America Oudan Ultra Quiz Part 2",
        "(Japan)",
        "DMG-AUJ",
        456274376,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "c9a1211515dcf89a982709b28a6d66b6a54d4f97",
        "Ultraman Ball",
        "(Japan) (SGB)",
        "DMG-AUMJ",
        1183652398,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "f64d3c0a0202654294e1581cdd34fa42c7ac93fd",
        "Uno",
        "(USA) (GB Comp.)",
        "DMG-AUNE",
        4029076745,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "845bb0ce9a4ea22884f408e4eb5eedb6a7b68866",
        "Uno",
        "(Europe) (En,Fr,De,Es,It,Nl) (GB Comp.)",
        "DMG-AUNP",
        1886784718,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "5a42a3b60fab9f9c204d2c1de17d7853625c1039",
        "Spirou",
        "(Europe) (En,Fr,De,Es) (SGB)",
        "DMG-AUOP",
        1935354338,
        262144,
        "En,Fr,De,Es",
        "Europe"
   , " " },
    {
        "8fe5b4f7c9872c08edc20a30b725c14aa66b32d5",
        "Urban Strike",
        "(USA, Europe) (SGB)",
        "DMG-AUSE",
        2304923973,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "9fd4b65d800f85c74611e06ccb3d05e338e6c8b5",
        "Rampage 2 - Universal Tour",
        "(USA, Europe)",
        "CGB-AUTE",
        548957982,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "72242f47c54b31f7e68eb3ee7986723220de2cfd",
        "Yu Yu Hakusho Dai-4-dan - Makai Touitsu Hen",
        "(Japan) (SGB)",
        "DMG-AUUJ",
        1048341291,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "586efe3acb0cdc09a9a98a5c7f79028f988c2e35",
        "WWF WrestleMania 2000",
        "(USA, Europe) (GB Comp.)",
        "DMG-AUWE",
        4240052910,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "ddb955bf988398907b86ba7e90ee43c57f7f962b",
        "SD Hiryuu no Ken EX",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AUXJ",
        911995967,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "88113582cc853ad1eaa5bcd257bbdbce58e29b0c",
        "Spacestation Silicon Valley",
        "(Europe) (En,Fr,De,Es,It,Nl,Sv) (GB Comp.)",
        "DMG-AV2P",
        1664433167,
        2097152,
        "En,Fr,De,Es,It,Nl,Sv",
        "Europe"
   , " " },
    {
        "843a9004f6eebc67479f76fea0d2585f78b32847",
        "Namco Gallery Vol.3",
        "(Japan) (SGB)",
        "DMG-AV3J",
        2000696350,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "be0c8c81815ae78cf2bfde3668c771fb54254491",
        "Bionic Commando - Elite Forces",
        "(USA, Australia)",
        "CGB-AV4E",
        2791558961,
        2097152,
        "En",
        "USA, Australia"
   , " " },
    {
        "5f100796f66a8352a302948582257eb783fb9750",
        "Pocket King",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AV5J",
        3739688260,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "35c85235a545e99eb5f921a44868e3088838ad28",
        "T2 - The Arcade Game",
        "(Japan)",
        "DMG-AVA",
        2127009274,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "d7af52e20cc78c04edcd78ddb9dc8917ad488fbe",
        "Babe and Friends",
        "(USA) (GB Comp.)",
        "DMG-AVAE",
        3797157873,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "7d7acdfddf79e6a4ef1c6214daf2efa73d028c89",
        "Babe and Friends",
        "(Europe) (En,Fr,De,Es,It) (GB Comp.)",
        "DMG-AVAP",
        3009943636,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "518f53cb9c510c19027f9ed8855184ed47630073",
        "Pocket Bowling",
        "(USA) (GB Comp.)",
        "DMG-AVBE",
        1054017800,
        524288,
        "En",
        "USA"
   , " " },
    {
        "4a791cb908dcd761a23f20e6dfbaa2246de4ab80",
        "Pocket Bowling",
        "(Japan) (GB Comp.)",
        "DMG-AVBJ",
        643341177,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "24b2e4da7b3f4a1ccefbfaea4e8879689e40b0e6",
        "Army Men",
        "(USA, Europe) (En,Fr,De)",
        "CGB-AVCE",
        876681043,
        1048576,
        "En,Fr,De",
        "USA, Europe"
   , " " },
    {
        "d48f1ad3d4fc5bf6ca23e7e5d5073d25eb59433a",
        "Pro Darts",
        "(USA)",
        "CGB-AVDE",
        2202694336,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "bf0bb9370872b2969d527675d2d717e016c4d77a",
        "T2 - The Arcade Game",
        "(USA, Europe)",
        "DMG-AVE",
        15768703,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "0ca4aabc6e11db5e4aed7b2ea8906e665934add7",
        "Pumuckls Abenteuer bei den Piraten",
        "(Germany)",
        "CGB-AVED",
        4116503417,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "58e3e2bc86f554a5889781187c9c492822688716",
        "Vegas Games",
        "(USA)",
        "CGB-AVFE",
        1085663894,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "96dc82ad44706969c53423d83b41159d0e5cdad5",
        "Vegas Games",
        "(Europe) (En,Fr,De)",
        "CGB-AVFP",
        2175974285,
        1048576,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "9e7fb7720179a40e6bd1080d78ee93113a7367f4",
        "Visiteurs, Les",
        "(France) (GB Comp.)",
        "DMG-AVGF",
        3628333208,
        1048576,
        "Fr",
        "France"
   , " " },
    {
        "92d98d08bcf1baa8ef8ad3edeed74e2a6584914a",
        "Beavis and Butt-head",
        "(USA, Europe)",
        "DMG-AVIE",
        2937774371,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "5b7e80299f418468f2ac8b151bcbdce38f0b2dd7",
        "V-Rally - Championship Edition",
        "(Japan)",
        "CGB-AVJJ",
        4206483777,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "150ee6c174e59b029cd0188e75f0c23d530e8f8c",
        "Survival Kids",
        "(USA) (SGB) (GB Comp.)",
        "DMG-AVKE",
        3295328854,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "c1f16ee1a45170eb72bdd998dc76546d1a0da442",
        "Survival Kids - Kotou no Boukensha",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AVKJ",
        1643800412,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "0c36ac34b82063ae254d711953162dc311f033fa",
        "Stranded Kids",
        "(Europe) (En,Fr,De) (SGB) (GB Comp.)",
        "DMG-AVKP",
        2171227540,
        1048576,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "1850045b3fdfcebe3219b5fb7d622dcdd04ef98a",
        "V-Rally - Edition 99",
        "(USA) (En,Fr,Es)",
        "CGB-AVLE",
        3660581996,
        1048576,
        "En,Fr,Es",
        "USA"
   , " " },
    {
        "b64d118ad406f2597bcde0382f161f7e6e6ff47c",
        "V-Rally - Championship Edition",
        "(Europe) (En,Fr,De)",
        "DMG-AVLP",
        3511248171,
        262144,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "6e8d73590dd596a3519d4a654eabdf2c819cd60d",
        "Rugrats - Time Travelers",
        "(USA, Europe) (GB Comp.)",
        "DMG-AVME",
        2624864003,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "c8a31f2d63e010c83a912aade5f68e31befa7fac",
        "BattleTanx",
        "(USA)",
        "CGB-AVNE",
        1144125671,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "7f082d24edaf8b5ea51ce6919f85aa3063eaaeff",
        "BattleTanx",
        "(Europe) (En,Fr,De)",
        "CGB-AVNP",
        802081648,
        1048576,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "b091b11ff9e20247c7dec64a7ade4c0992599942",
        "Taito Variety Pack",
        "(Japan)",
        "DMG-AVPJ",
        1840948712,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "6eff28237a609bc97cf3c4e6f3e9d8f5cf0b789f",
        "Bomberman Quest",
        "(USA) (SGB) (GB Comp.)",
        "DMG-AVQE",
        2693383766,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "483d81b84855f5df3f556d8e1914d0402fef06c3",
        "Vegas Stakes",
        "(USA, Europe) (SGB)",
        "DMG-AVSE",
        3367896536,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "c893a9ec34609e16edb64ff5d5aa2940cc7a305b",
        "Rats!",
        "(USA) (En,Es) (GB Comp.)",
        "DMG-AVTE",
        392387281,
        524288,
        "En,Es",
        "USA"
   , " " },
    {
        "85448c3992e582827a590052572d7b005cfc4cb2",
        "Reservoir Rat",
        "(Europe) (En,Fr,De,Es,It) (GB Comp.)",
        "DMG-AVTP",
        576854019,
        524288,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "7d83944a010b04bb9cafb9b1c4f1080055cd0cdc",
        "Beauty and the Beast - A Board Game Adventure",
        "(USA, Australia) (SGB) (GB Comp.)",
        "DMG-AVUE",
        1843516801,
        1048576,
        "En",
        "USA, Australia"
   , " " },
    {
        "9a92737fe5243fbe82676f34a3dff84304c9e5a3",
        "Beauty and the Beast - A Board Game Adventure",
        "(Europe) (En,Fr,De,Es,It) (SGB) (GB Comp.)",
        "DMG-AVUP",
        3838054640,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "0e8324a40f456933c1cd5667d2fa0908eb62f15a",
        "Power Pro Kun Pocket",
        "(Japan) (Rev 1) (SGB) (GB Comp.)",
        "DMG-AVVJ",
        2303559922,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "76b22bc1da9b44c4d240cadc86090ee005bde94b",
        "Power Pro Kun Pocket",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AVVJ",
        341131480,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "b36eb7abf4d13174dd9d24b7cbbb04013d7942cd",
        "Classic Bubble Bobble",
        "(USA) (SGB) (GB Comp.)",
        "DMG-AVWE",
        3249676870,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "cfb784ffba1fa60239a3206c43a6031796da3486",
        "Classic Bubble Bobble",
        "(Europe) (SGB) (GB Comp.)",
        "DMG-AVWP",
        2700504455,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "6283de31d02d1ea5170d435cf0c497c6eec91625",
        "Shamus",
        "(USA, Europe) (GB Comp.)",
        "DMG-AVXE",
        4021885293,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "809435f923c3d40444069a91b8ba396ffb974ecb",
        "V-Rally - Championship Edition",
        "(Europe) (En,Fr,De,Es)",
        "CGB-AVYP",
        319992823,
        1048576,
        "En,Fr,De,Es",
        "Europe"
   , " " },
    {
        "2680dabad1c71a77ad60510f3344e33af2ea7f3b",
        "Tasmanian Devil - Munching Madness",
        "(Europe) (En,Fr,De,Es,It) (GB Comp.)",
        "DMG-AVZP",
        1748456096,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "9b3b8a95ca26f3e17351d3a9007fdcbf5c5c46c0",
        "Wario Land II",
        "(USA, Europe) (SGB)",
        "DMG-AW2E",
        2622764429,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "2124cdad1a9d3d0842b561b35f3b952e8fa522aa",
        "Wario Land 2",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AW2J",
        3004161013,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "e0f36e0099838698e9cfa2601269261f72354fb0",
        "Worms",
        "(Europe)",
        "DMG-AW3E",
        784089282,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "82050ea21d8e9a3760636d647796d68040f95382",
        "Goukaku Boy Series - Yamakawa Ichimonittou - Sekaishi B Yougo Mondaishuu",
        "(Japan)",
        "DMG-AW4J",
        3515201849,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "1b3de63a3ce24abbba52e0a0fdf20f7e129141c7",
        "Wetrix GB",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AW5J",
        1645594035,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "99574ed246599c55f38729259e5677be4a47b6a3",
        "Test Drive 6",
        "(USA) (GB Comp.)",
        "DMG-AW6E",
        1558185287,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "b07110651cf945e4e34ebd1260d373486b673f20",
        "Test Drive 6",
        "(Europe) (En,Fr,De,Es,It) (GB Comp.)",
        "DMG-AW6P",
        325655787,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "ddd5a69a3e34a419a775d78fe5c06f6afb564c47",
        "James Bond 007",
        "(USA, Europe) (SGB)",
        "DMG-AW7E",
        3392914382,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "a1a2afea701bc6d45008a7eca5b182f14734d297",
        "Wario Land 3",
        "(World) (En,Ja)",
        "CGB-AW8A",
        1208812121,
        2097152,
        "En,Ja",
        "World"
   , " " },
    {
        "af808963f3b72ac49e5d89a578b31f74e3ff1f3d",
        "Wacky Races",
        "(USA) (En,Fr,Es)",
        "CGB-AW9E",
        1413135131,
        1048576,
        "En,Fr,Es",
        "USA"
   , " " },
    {
        "ffa520d883838a0ba2e35745ecf18ae2bc28fa6a",
        "Chiki Chiki Machine Mou Race",
        "(Japan)",
        "CGB-AW9J",
        918572000,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "bf5b758b8b501c6d1af9e0a7dea87cbe5c154251",
        "Wacky Races",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-AW9P",
        938107027,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "cc7b2890e19d11ccd9374870a2369925eff7f9c8",
        "Alleyway",
        "(World)",
        "DMG-AWA",
        1556092294,
        32768,
        "En",
        "World"
   , " " },
    {
        "bf59ffa1fe996c844a237265af48584be4cfc5e9",
        "Chou Mashin Eiyuu Den Wataru - Mazekko Monster",
        "(Japan) (SGB)",
        "DMG-AWAJ",
        3018326669,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "21f5eae6e99dc5d199547d5f5354ca020f4a5da5",
        "Bugs Bunny Collection",
        "(Japan) (Rev 1) (SGB)",
        "DMG-AWBJ",
        2185503243,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "53bdac5caab1bf3fd4298828593bc8c1ca8dd27c",
        "Bugs Bunny Collection",
        "(Japan) (SGB)",
        "DMG-AWBJ",
        3537627791,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "279d31fe4779547a0281784f84530cf2549ada04",
        "Dino Breeder",
        "(Japan) (SGB)",
        "DMG-AWDJ",
        1057665004,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "cb0252b237e8890a4030f7f575bbe08b2237e9b8",
        "Dino Breeder",
        "(Japan) (Rev 1) (SGB)",
        "DMG-AWDJ",
        1529387700,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "bc7d0d55467dd23e284e33d3a5c418b83f44c74e",
        "Waterworld",
        "(Europe)",
        "DMG-AWEE",
        1200029038,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "d4806bf9bfc7b6516bf9c0e7471025cdc9a4ae8e",
        "WWF Raw",
        "(USA, Europe)",
        "DMG-AWFE",
        411653423,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "8c7c84c8c248a4ad8c137920efd42a6b86132a42",
        "J.League Live 95",
        "(Japan) (SGB)",
        "DMG-AWGJ",
        4029813570,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "ef52e232b77f36fea488646686b7adc2105e3cb1",
        "R-Type DX",
        "(USA, Europe) (GB Comp.)",
        "DMG-AWHE",
        4229775497,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "61a092ee82a1f080dfaa1e97ad5174da7a5a11a3",
        "Soukoban Densetsu - Hikari to Yami no Kuni",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AWIJ",
        136150987,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "04ce2e98f79ec5ecd2e4583754a8b2414b44c246",
        "World Heroes 2 Jet",
        "(USA, Europe) (SGB)",
        "DMG-AWJE",
        1731213652,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "d2d56ba13d319b21cccd07068cb97725ab0a8f81",
        "Nettou World Heroes 2 Jet",
        "(Japan) (Rev 1) (SGB)",
        "DMG-AWJJ",
        2086665810,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "d4b7253d7e377a2189d709efbb22bbff26f0391e",
        "Nettou World Heroes 2 Jet",
        "(Japan) (SGB)",
        "DMG-AWJJ",
        169014588,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "afd5bb5cf2fbf43d5d429c7f1ccb2e691d9e2620",
        "Kanji Boy",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AWKJ",
        415933715,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "5e963e1ba0d3ac180c9dbc7b7abfb3dc6e6ef9af",
        "Wario Land II",
        "(USA, Europe) (SGB) (GB Comp.)",
        "DMG-AWLE",
        75227008,
        2097152,
        "En",
        "USA, Europe"
   , " " },
    {
        "f47a441dfb8b7a3353a91174adfe93eb149b2748",
        "Chou Mashin Eiyuu Den Wataru - Mazekko Monster 2",
        "(Japan) (SGB)",
        "DMG-AWMJ",
        3847358099,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "46edd1db6923a6e76ce8bd885821c2c04b4dca01",
        "Uchiiwai - Kyoudaijingi no Puzzle Game",
        "(Japan)",
        "DMG-AWNJ",
        2153364501,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "20bd363b0e60acdc6cf2e490ec40a7b7f9aecae0",
        "Game Boy Wars 2",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AWOJ",
        3332227963,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "fc50f99800a8a0530ac1661a056718ee735c47b5",
        "Wedding Peach - Jamapii Panic",
        "(Japan) (SGB)",
        "DMG-AWPJ",
        2551359174,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "751330eda7b51610013c049454d85b85827b005c",
        "Dragon Quest Monsters",
        "(Germany) (SGB) (GB Comp.)",
        "DMG-AWQD",
        713209403,
        2097152,
        "De",
        "Germany"
   , " " },
    {
        "6d8184881466ca3ae58806b17e6baab254bd0a5b",
        "Dragon Warrior Monsters",
        "(USA, Europe) (SGB) (GB Comp.)",
        "DMG-AWQE",
        3849074097,
        2097152,
        "En",
        "USA, Europe"
   , " " },
    {
        "59dab34db3f3354fc38606f3939519d577d2b4ab",
        "Kakutou Ryouri Densetsu Bistro Recipe - Gekitou Foodon Battle Hen",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AWRJ",
        1167725163,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "272ea21cd8a7e72b3e414acb38ef66797b3b65fa",
        "Wild Snake",
        "(USA) (SGB)",
        "DMG-AWSE",
        1917198629,
        65536,
        "En",
        "USA"
   , " " },
    {
        "34ad3626e8bec4f95cf832d5a0e34af39df7a7c3",
        "Super Snakey",
        "(Japan) (SGB)",
        "DMG-AWSJ",
        2555187071,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "ed94809a93cbb9d8fad5ca1489ff3ba3a0497c3a",
        "WWF Attitude",
        "(USA, Europe)",
        "CGB-AWTE",
        3590190730,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "30a65921bd70c60c34f0862e11a0735f860a1e8b",
        "Worms Armageddon",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-AWUP",
        3406099277,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "5cd4bb8aca240af1678379dfc97716d680c5fb6a",
        "WWF War Zone",
        "(USA, Europe)",
        "DMG-AWWE",
        1612036065,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "d8d1d9eed33cbf2aa7378c32c164bf5152952854",
        "Mario Golf",
        "(USA)",
        "CGB-AWXE",
        2421870795,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "b33ef53a5ae3b2d42634864a10ef62c40771fcfa",
        "Mario Golf GB",
        "(Japan)",
        "CGB-AWXJ",
        1285691674,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "2dfee7cdcd2e1b485abfdc69267642e8f65e3d1e",
        "Mario Golf",
        "(Europe)",
        "CGB-AWXP",
        2704425341,
        2097152,
        "En",
        "Europe"
   , " " },
    {
        "855a8b8d185b1f61a3e8293edc8681951ae3e005",
        "Puyo Puyo Gaiden - Puyo Wars",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AWYJ",
        1731529673,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "e6a58a77184afa542d350122c20540e206d605c3",
        "International Superstar Soccer 99",
        "(USA) (SGB) (GB Comp.)",
        "DMG-AWZE",
        2634191015,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "0ab3afb24e57b25891efcf126c9455c86f1bb31c",
        "World Soccer GB 2",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AWZJ",
        3709393759,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "2b6a46f813fd1b59add9e90ff71a34e9d48cf986",
        "International Superstar Soccer 99",
        "(Europe) (SGB) (GB Comp.)",
        "DMG-AWZP",
        460771605,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "17c18139189e95c922e6f351280ba773c008f5d1",
        "NFL Blitz 2000",
        "(USA)",
        "CGB-AX2E",
        151813548,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "17c30b826514881e0bb961e7ef6fe57bc8d1461c",
        "Jim Henson's Muppets",
        "(Europe) (En,Fr,De,Es,It,Nl,Sv)",
        "CGB-AX9P",
        263169010,
        2097152,
        "En,Fr,De,Es,It,Nl,Sv",
        "Europe"
   , " " },
    {
        "72cb2e97a0514afec11d61116bf40dd5a7b93fe0",
        "Pachinko CR Mouretsu Genshijin T",
        "(Japan) (GB Comp.)",
        "DMG-AXAJ",
        4150142622,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "110846a30c371d14cb0258916c6c48ac96d57449",
        "Magical Tetris Challenge",
        "(Europe) (En,Fr,De,Es,It,Nl,Sv) (Rev 1)",
        "CGB-AXCP",
        3358319348,
        1048576,
        "En,Fr,De,Es,It,Nl,Sv",
        "Europe"
   , " " },
    {
        "b716ef055ad9e3b4c296f56a038c3c7520310a7c",
        "Magical Tetris Challenge",
        "(Europe) (En,Fr,De,Es,It,Nl,Sv)",
        "CGB-AXCP",
        172103737,
        1048576,
        "En,Fr,De,Es,It,Nl,Sv",
        "Europe"
   , " " },
    {
        "f48c456616dd684d90a268617ccb552dcf9d60c2",
        "New Adventures of Mary-Kate & Ashley, The",
        "(USA, Europe) (GB Comp.)",
        "DMG-AXFE",
        3701753678,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "26e9d8f26e460858103dc3daa2b363d8124b6270",
        "Gex 3 - Deep Pocket Gecko",
        "(USA)",
        "CGB-AXGE",
        2242481277,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "894729a499a016c1a06f0aeaf020a6f3466e9106",
        "Gex 3 - Deep Cover Gecko",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-AXGP",
        39792733,
        2097152,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "6fa3924c473200bbd90221cdd0a9e71d601fd811",
        "Deer Hunter",
        "(USA)",
        "CGB-AXHE",
        1084691963,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "2f9a044e8a820b542f3f087c2a258b1452885910",
        "Iron Man X-O Manowar in Heavy Metal",
        "(USA, Europe) (SGB)",
        "DMG-AXIE",
        389165797,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "30d76180a5e90af4484b86ccce41d921e5b77144",
        "Magical Drop",
        "(USA)",
        "CGB-AXJE",
        3826813561,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "5e4889dcefb8f4a3022adcf09d3c5197113f456e",
        "Magical Drop",
        "(Europe) (En,Fr,De)",
        "CGB-AXJP",
        3936281091,
        1048576,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "f7d59cd5bb4a50a4fe27a3560626748ef4282037",
        "Knockout Kings",
        "(USA, Europe)",
        "CGB-AXKE",
        2698398004,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "20f7840a67cce12676d254a023af6683e39fa5e4",
        "Star Sweep",
        "(Japan) (SGB)",
        "DMG-AXLJ",
        3606943356,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "24ec2a7469a571676a91f39b08f40cd2a5a85b72",
        "Asterix & Obelix",
        "(Europe) (Fr,De) (SGB)",
        "DMG-AXOP",
        1363403303,
        262144,
        "Fr,De",
        "Europe"
   , " " },
    {
        "5752fbc88777540a222654e723e8d6108784ff08",
        "Asterix & Obelix",
        "(Spain) (En,Es) (SGB)",
        "DMG-AXOS",
        2986164688,
        262144,
        "En,Es",
        "Spain"
   , " " },
    {
        "57f05ed150227f3c2cd403b2b039257eb3ad6b10",
        "10-Pin Bowling",
        "(Europe)",
        "CGB-AXPP",
        1414122176,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "52-9894",
        "Pokemon Trading Card Game",
        "(USA, Australia) (SGB) (GB Comp.)",
        "DMG-AXQE",
        2164694355,
        1048576,
        "En",
        "USA, Australia",
        "/pokemon_tcg64.sprite" 
    },
    {
        "40-24098",
        "Pokemon Trading Card Game",
        "(Europe) (En,Fr,De) (SGB) (GB Comp.)",
        "DMG-AXQP",
        1159935854,
        2097152,
        "En,Fr,De",
        "Europe",
        "/pokemon_tcg64.sprite" 
    },
    {
        "39-18893",
        "Pokemon Trading Card Game",
        "(Europe) (En,Fr,De) (Rev 1) (SGB) (GB Comp.)",
        "DMG-AXQP",
        2485980031,
        2097152,
        "En,Fr,De",
        "Europe",
        "/pokemon_tcg64.sprite" 
    },
    {
        "31-46144",
        "Pokemon Trading Card Game",
        "(Europe) (En,Es,It) (Rev 1) (SGB) (GB Comp.)",
        "DMG-AXQX",
        1058897496,
        2097152,
        "En,Es,It",
        "Europe",
        "/pokemon_tcg64.sprite" 
    },
    {
        "32-51305",
        "Pokemon Trading Card Game",
        "(Europe) (En,Es,It) (SGB) (GB Comp.)",
        "DMG-AXQX",
        2523770609,
        2097152,
        "En,Es,It",
        "Europe",
        "/pokemon_tcg64.sprite" 
    },
    {
        "e8471ad00d2b926e34288bfc6f9736559c55b0da",
        "Suzuki Alstare Extreme Racing",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-AXRP",
        259155130,
        524288,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "a26a19f3d604a20acd19bc56df88d931160cb41e",
        "Tonic Trouble",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-AXTP",
        2825031546,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "c55743ad4475892951a157ec3470b55e7e4fa163",
        "Boarder Zone",
        "(USA)",
        "CGB-AXWE",
        2803181673,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "1fa770e58cd7fd04e85e4d401d919ce50586f4f8",
        "Supreme Snowboarding",
        "(Europe) (En,Fr,De)",
        "CGB-AXWP",
        1404167777,
        2097152,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "e42973da2ee6ac8a04ed215fe66a42258f5c50e6",
        "Pachinko Data Card - Chou Ataru-kun",
        "(Japan) (SGB)",
        "DMG-AXXJ",
        2505778618,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "6cc6ff3c4cd233709d8ac41c7ebecf0bd9c4b9d7",
        "Rip-Tide Racer",
        "(Europe) (En,Fr,De,Es,It) (GB Comp.)",
        "DMG-AXYP",
        2878093873,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "b9b2c5a53c38fc3e751a401241fb95e4b1dfb0d4",
        "Nada Asatarou no Powerful Mahjong - Tsugi no Itte 100 Dai",
        "(Japan)",
        "DMG-AY2J",
        3034418794,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "25fa7e5b015f3aa0e327e7fd3dcb3695fd40fc17",
        "Goukaku Boy Series - Gakken - Yojijukugo 288",
        "(Japan)",
        "DMG-AY3J",
        1662678318,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "98aada98c9ae8f308d9df9371dbd9d5ef3bea58c",
        "Goukaku Boy Series - Gakken - Yojijukugo 288",
        "(Japan) (Rev 1)",
        "DMG-AY3J",
        3370130556,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "573a582e2a5492dc5cfcde75d4fbfe618ea079a9",
        "Otogibanashi Taisen",
        "(Japan) (SGB)",
        "DMG-AY7J",
        2555381025,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "87b11b11fcf2a69084e8b9143b26c4e787ca6690",
        "Papyrus",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-AY9P",
        3661247096,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "f960da9041c3d8cd836664aea53b12cbe9575772",
        "Ken Griffey Jr.'s Slugfest",
        "(USA)",
        "CGB-AYAE",
        509923740,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "3284d7903aa0a1e2b2d7f839e7c37f3275985902",
        "Yu-Gi-Oh! - Monster Capsule GB",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AYCJ",
        326232178,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "e0eaad0e637a503f2b8f60076a44d035cbb06231",
        "Star Wars - Yoda Stories",
        "(USA, Europe) (GB Comp.)",
        "DMG-AYDE",
        1662310962,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "0b3d00ef184e3dffeffcf4e64e1f44fc8a65e1f7",
        "Pachinko Hisshou Guide - Data no Ousama",
        "(Japan) (GB Comp.)",
        "DMG-AYEJ",
        442702763,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "ffcc0ca463eb77e46e6792988ed193b2f5913fdc",
        "Tiger Woods PGA Tour 2000",
        "(USA, Europe) (GB Comp.)",
        "DMG-AYGE",
        2799677913,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "3a1fbf1e9eb417a4ef14e29fa0b462154a90e372",
        "WCW Mayhem",
        "(USA, Europe)",
        "CGB-AYHE",
        2676367569,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "f086a532802fda1e4fd45b1afd7644b2b7eec0b4",
        "Owarai Yoiko no Geemumichi - Oyaji Sagashite 3 Choume",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AYIJ",
        1063475791,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "2525659f08d165c19882054e10c1d526a460f6fa",
        "Ayakashi no Shiro",
        "(Japan)",
        "DMG-AYJ",
        3666933244,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "ecadd4c0468b1d2881b514133ad325543aa887ab",
        "Crystalis",
        "(USA)",
        "CGB-AYJE",
        2426122285,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "5f6606c383885450271c11aaa6891ac0a2d20074",
        "Yu-Gi-Oh! Duel Monsters II - Dark Duel Stories",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AYKJ",
        3128001219,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "1a3298d4c51447274d0d6be32ac7ef9d22246b5f",
        "Tetris Attack",
        "(USA, Europe) (SGB)",
        "DMG-AYLE",
        3077338779,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "e215487aaf84d1dcc3ddc802bd620d213e1ee2b9",
        "Tetris Attack",
        "(USA, Europe) (Rev 1) (SGB)",
        "DMG-AYLE",
        1592023629,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "0a980c283f87924c75e5bda497a9f9528cf62e0c",
        "Yoshi no Panepon",
        "(Japan) (SGB)",
        "DMG-AYLJ",
        1005285945,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "a409c662fe9342b3c7467a30959e9d843bedcda6",
        "Gakkyuu Ou Yamazaki",
        "(Japan) (Rev 1) (GB Comp.)",
        "DMG-AYMJ",
        4012703129,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "421d7e6af8540604052872539bfddae651bc5a10",
        "Yannick Noah Tennis",
        "(France) (En)",
        "DMG-AYNP",
        1076710727,
        65536,
        "En",
        "France"
   , " " },
    {
        "c1ef8dddeba1e685bf16bdf78f9ade5901b50c97",
        "Paperboy",
        "(USA, Europe)",
        "CGB-AYPE",
        3232334597,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "90c255111388d454337ee7d9d053b89d3ed43b89",
        "Rayman",
        "(USA) (En,Fr,De,Es,It,Nl)",
        "CGB-AYQE",
        3986763533,
        4194304,
        "En,Fr,De,Es,It,Nl",
        "USA"
   , " " },
    {
        "d578eec3bdbc4c2e80b5c0583492c3aa030cc0c7",
        "Rayman",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-AYQP",
        3291523226,
        4194304,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "c08042ded43a887469930d849ec629d227774f0a",
        "Looney Tunes - Twouble!",
        "(USA) (En,Fr,Es) (GB Comp.)",
        "DMG-AYRE",
        1770879972,
        1048576,
        "En,Fr,Es",
        "USA"
   , " " },
    {
        "db8cb75c1a8274b9f301124405597ed706fe4d8e",
        "Sylvester & Tweety - Breakfast on the Run",
        "(Europe) (En,Fr,De,Es,It,Nl) (GB Comp.)",
        "DMG-AYRP",
        1806938332,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "de6f32305bf9a70c80220875a12bf889af78d506",
        "Pocket Puyo Puyo Sun",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AYSJ",
        1164320451,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "b02908c7d3957a43096d8be66c8c690660ce5ab7",
        "Yakouchuu GB",
        "(Japan)",
        "CGB-AYTJ",
        3768606744,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "58714be783408d35e1ccf86d24bb2a5cf7778746",
        "Yu-Gi-Oh! Duel Monsters",
        "(Japan) (SGB)",
        "DMG-AYUJ",
        2289429588,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "4cc764c73f976b795d8137568e290df48f7ea3a4",
        "Yars' Revenge",
        "(USA, Europe) (GB Comp.)",
        "DMG-AYVE",
        3600966724,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "9349d4551f25dcee394e3adeb56161ed44274dd3",
        "Harvest Moon GB",
        "(USA) (SGB)",
        "DMG-AYWE",
        514723344,
        524288,
        "En",
        "USA"
   , " " },
    {
        "60c2cc427b8b3f6f010bdf6b66bcb9c8e88c18b6",
        "Bokujou Monogatari GB",
        "(Japan) (Rev 1) (SGB)",
        "DMG-AYWJ",
        3659632196,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "693ec29353c1b8b08589e063730260da1bc2c6d3",
        "Bokujou Monogatari GB",
        "(Japan) (SGB)",
        "DMG-AYWJ",
        3554868482,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "cc7154cf3220264dfe5e7cb8d3b398b5fddc5efb",
        "Harvest Moon GB",
        "(Germany) (SGB) (GB Comp.)",
        "DMG-AYXD",
        3548997202,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "1056438d87af6c36997c2cb3d8b2548dcb962a35",
        "Harvest Moon GBC",
        "(USA) (SGB) (GB Comp.)",
        "DMG-AYXE",
        2874620065,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "ab8a13f495ebee88e53e86d2b1b361a2a15e1a4a",
        "Harvest Moon GB",
        "(Europe) (SGB) (GB Comp.)",
        "DMG-AYXP",
        3366385290,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "558cc7a39c8d1052e902b31511262f5aa957486f",
        "Chousoku Spinner",
        "(Japan) (SGB)",
        "DMG-AYYJ",
        3036323058,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "000a86d0c7e03a231caa101fa7533b3feffaaeaa",
        "Dragon Ball Z - Gokuu Gekitouden",
        "(Japan) (SGB)",
        "DMG-AZ2J",
        4282122877,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "389efb400d69859d8bc55e871b51f10d5cbfe586",
        "Goukaku Boy Series - Z-Kai Kyuukyoku\n no Eijukugo 1017",
        "(Japan)",
        "DMG-AZ3J",
        317143274,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "85310a895898506d15a68130373b4e80ac35af8d",
        "Legend of Zelda, The - Oracle of Seasons",
        "(USA, Australia)",
        "CGB-AZ7E",
        3622434263,
        1048576,
        "En",
        "USA, Australia"
   , " " },
    {
        "234-6569",
        "Zelda no Densetsu - Fushigi no Kinomi\n - Daichi no Shou",
        "(Japan)",
        "CGB-AZ7J",
        3827644656,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "95107a5257def79858ef05fd79bd230d88f3ce12",
        "Legend of Zelda, The - Oracle of Seasons",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-AZ7P",
        3685487447,
        2097152,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "94ecd9a4390d2e760b9eaf33fc45e96c4cd92a4b",
        "Legend of Zelda, The - Oracle of Ages",
        "(USA, Australia)",
        "CGB-AZ8E",
        939565959,
        1048576,
        "En",
        "USA, Australia", 
        "/zelda_ages64.sprite" },
    {
        "53-38181",
        "Zelda no Densetsu - Fushigi no Kinomi\n - Jikuu no Shou",
        "(Japan)",
        "CGB-AZ8J",
        846391033,
        1048576,
        "Ja",
        "Japan", 
        "/zelda_ages64.sprite" },
    {
        "d90d9a2b996a6c3a56e3bdb68e205ce84025eafe",
        "Legend of Zelda, The - Oracle of Ages",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-AZ8P",
        1496572922,
        2097152,
        "En,Fr,De,Es,It",
        "Europe", 
        "/zelda_ages64.sprite" },
    {
        "f44c182c1b0f6d4e501db2de4152b7217791a57e",
        "Golf Ou - The King of Golf",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AZAJ",
        1665871171,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "37835295e80508d2f43219ca415da99341b62967",
        "Catz",
        "(USA)",
        "CGB-AZCE",
        1989815386,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "4b60f98254f5c168cf91927df5b0d677d726ac02",
        "Catz",
        "(Europe)",
        "CGB-AZCP",
        161310037,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "c64652b9c97f6a8d50ee78f903aa58dd8bbe7e27",
        "Front Row",
        "(Japan)",
        "CGB-AZDJ",
        1860866627,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "f62cf1291abd41866b5921f3f215af7250e04af6",
        "Noddy and the Birthday Party",
        "(Europe) (En,Fr,De,Es)",
        "CGB-AZEP",
        2220576324,
        1048576,
        "En,Fr,De,Es",
        "Europe"
   , " " },
    {
        "80e71f5e7b068f7f54def042272d149dc0f78625",
        "Jinsei Game",
        "(Japan) (SGB)",
        "DMG-AZGJ",
        167066965,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "edef6c0b589ba429d3f62da49d490a39e05356b9",
        "Godzilla - The Series",
        "(USA) (En,Fr,De) (GB Comp.)",
        "DMG-AZIE",
        3715198718,
        1048576,
        "En,Fr,De",
        "USA"
   , " " },
    {
        "7e387eca9d4e8f0c35a5b0201c8adc2103888419",
        "Godzilla - The Series",
        "(Europe) (En,Fr,De) (GB Comp.)",
        "DMG-AZIP",
        705114985,
        1048576,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "b6b1987320bd832e2934493fa9ad7ebd9563eef0",
        "Aretha III",
        "(Japan)",
        "DMG-AZJ",
        1124941163,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "47f96757133322e11c58ba0b05d106d7832dae6b",
        "Ou Dorobou Jing - Devil Version",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AZKJ",
        708421748,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "5b30e7e0283718f4329514136852d27dbc1ed752",
        "Legend of Zelda, The - Link's Awakening DX",
        "(Germany) (SGB) (GB Comp.)",
        "DMG-AZLD",
        4275410331,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "e9d55652657ff2bd4d3ef415ffa21ae6e74393de",
        "Legend of Zelda, The - Link's Awakening DX",
        "(Germany) (Rev 1) (SGB) (GB Comp.)",
        "DMG-AZLD",
        4021774199,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "acc0671e8b4602078ed52c5cfa036ec616ac1c74",
        "Legend of Zelda, The - Link's Awakening DX",
        "(USA, Europe) (Rev 1) (SGB) (GB Comp.)",
        "DMG-AZLE",
        3012475358,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "c1fa639c4081ee38d168bd4e59d9358e6ac333b3",
        "Legend of Zelda, The - Link's Awakening DX",
        "(USA, Europe) (SGB) (GB Comp.)",
        "DMG-AZLE",
        2541889864,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "ed389348c7a0530a87e986d601a49b742435d58d",
        "Legend of Zelda, The - Link's Awakening DX",
        "(USA, Europe) (Rev 2) (SGB) (GB Comp.)",
        "DMG-AZLE",
        109607476,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "5c61bf0728ed1b04f1e749815da84d69ea9cb758",
        "Legend of Zelda, The - Link's Awakening DX",
        "(France) (Rev 1) (SGB) (GB Comp.)",
        "DMG-AZLF",
        1311471079,
        1048576,
        "Fr",
        "France"
   , " " },
    {
        "79a0bf790984502fbb26f102b5cefe463e4890ac",
        "Legend of Zelda, The - Link's Awakening DX",
        "(France) (SGB) (GB Comp.)",
        "DMG-AZLF",
        4102563070,
        1048576,
        "Fr",
        "France"
   , " " },
    {
        "38b68f58a318818809b628a01ff473508d1d1e7f",
        "Zelda no Densetsu - Yume o Miru Shima DX",
        "(Japan) (Rev 2) (SGB) (GB Comp.)",
        "DMG-AZLJ",
        3919111573,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "9d9ecb7d1afe71099f021f425199a26cad9488cf",
        "Zelda no Densetsu - Yume o Miru Shima DX",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AZLJ",
        3648302058,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "e153a1b0b0862dc23657450e3d232594d43cd73f",
        "Zelda no Densetsu - Yume o Miru Shima DX",
        "(Japan) (Rev 1) (SGB) (GB Comp.)",
        "DMG-AZLJ",
        3179941953,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "1e445962fb465e122969324c7d0e52792abf895f",
        "Montezuma's Return!",
        "(USA) (En,Es) (GB Comp.)",
        "DMG-AZME",
        3724834607,
        524288,
        "En,Es",
        "USA"
   , " " },
    {
        "fc16c024f95d8327cd630c46683bb57de3c5213f",
        "Montezuma's Return!",
        "(Europe) (En,Fr,De,Es,It) (GB Comp.)",
        "DMG-AZMP",
        1704367070,
        524288,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "ee9e46d51401131c4a5f3b30d36e3e2f586413e2",
        "Zoop",
        "(USA, Europe)",
        "DMG-AZPE",
        351844027,
        65536,
        "En",
        "USA, Europe"
   , " " },
    {
        "5e90d5552a6998dc15dd78fb96a462a7c80548c7",
        "Zoop",
        "(Japan) (En)",
        "DMG-AZPJ",
        283222284,
        65536,
        "En",
        "Japan"
   , " " },
    {
        "604c8be3949ba822f41fa3ca9ec46cd2ba6ae4a1",
        "Puzzle Master",
        "(USA) (GB Comp.)",
        "DMG-AZQE",
        116095489,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "2d06997766d984ec162747cc8a7c53323d4725d8",
        "Mask of Zorro, The",
        "(USA)",
        "CGB-AZRE",
        747810066,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "0bb61bc29e4272fbf5ee78c68435b74f9db191df",
        "Mask of Zorro, The",
        "(Europe)",
        "CGB-AZRP",
        2426177295,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "3d35e2fbca6cc5a1a2fc73c9dae651f0adda67d1",
        "All Star Tennis 2000",
        "(Europe) (AZTP) (GB Comp.)",
        "DMG-AZTP",
        2502661349,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "e3c23e4c4dd772a2e9ece36f125744001bd98c5e",
        "Microsoft - The 6 in 1 Puzzle Collection Entertainment Pack",
        "(USA)",
        "CGB-AZWE",
        125286615,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "533f159622a5d4052e5e73242221de3c4596e88f",
        "Ou Dorobou Jing - Angel Version",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AZWJ",
        3564899791,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "a20d5dbec8e7826c9093f336398a51db9b60ab4e",
        "Puzzled",
        "(Europe) (En,Fr,De)",
        "CGB-AZWP",
        364137832,
        1048576,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "f4289d45d71431288537370b3ad6f87c7526887f",
        "Juukou Senki Bullet Battlers",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-AZXJ",
        1077526919,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "9a5672c6ed8cc565a60fac913862d88ab8b6a17c",
        "Space Marauder",
        "(USA)",
        "CGB-AZYE",
        1334031198,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "92b106e02cc61fad391f26affd50c9ec975a88ad",
        "Burai Senshi Color",
        "(Japan)",
        "CGB-AZYJ",
        3845111058,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "33a8afbd78b3b1f64aa20aa3c4590d6002094bb2",
        "Pro Mahjong Kiwame GB",
        "(Japan) (SGB)",
        "DMG-AZZJ",
        1868261192,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "5d3dab35961e2cac7310a264c4d8e994cfb3ccbc",
        "F-18 Thunder Strike",
        "(USA, Europe)",
        "CGB-B18E",
        1091545176,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "651497b515e1155001c29691d2a8108a3fc9ea2f",
        "Pia Carrot e Youkoso!! 2.2",
        "(Japan)",
        "CGB-B22J",
        577189387,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "06bfca864f51cafca4917a327f6479652c936b53",
        "Kisekae Series 2 - Oshare Nikki",
        "(Japan) (Rev 1)",
        "CGB-B23J",
        3910480764,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "e621faafdc3c7618f547e0be29f06ed12b9d4bd0",
        "Kisekae Series 2 - Oshare Nikki",
        "(Japan)",
        "CGB-B23J",
        2492418710,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "d6cf2557a0e5348ad1dda762495fbd989da68b05",
        "Test Drive Le Mans",
        "(USA) (En,Fr,Es)",
        "CGB-B24E",
        3872828559,
        1048576,
        "En,Fr,Es",
        "USA"
   , " " },
    {
        "56d9cd9e68e6ccb4849e3069e80ed888e890965d",
        "Le Mans 24 Hours",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-B24P",
        457822333,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "941228aaf5da5c95b118f31923748657213ae445",
        "Bubble Bobble",
        "(Japan)",
        "DMG-B2A",
        2912628748,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "977f4728f18be627a8503e763215a3cd6d7bf1ca",
        "Bomberman Selection",
        "(Korea)",
        "CGB-B2CK",
        2938847854,
        1048576,
        "Ko",
        "Korea"
   , " " },
    {
        "ec4ad9c98e9f61f72e11e392d5da55329c3c3b67",
        "Bubble Bobble",
        "(USA, Europe)",
        "DMG-B2E",
        3575022621,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "123675ba0a1d49124902f5b167fd85bb5ee4ee90",
        "Nakayoshi Pet Series 5 - Kawaii Hamster 2",
        "(Japan)",
        "CGB-B2HJ",
        3857480677,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "4c31476b2d2bd197945783efb745a110bfbe03eb",
        "Madden NFL 2002",
        "(USA)",
        "CGB-B2ME",
        3725801682,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "9c43d800745cf2fc1f737b5d0eb3073228b0bfea",
        "Mummy Returns, The",
        "(USA)",
        "CGB-B2RE",
        3382178907,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "5d9d0bf859af41f7fb5493598a62f1ff46086c9c",
        "Mummy Returns, The",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-B2RP",
        1678060443,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "b7f4de61af15b386893c6351cf717eec687b0edd",
        "Spider-Man 2 - The Sinister Six",
        "(USA, Europe)",
        "CGB-B2SE",
        2818223311,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "53f9e5a5e0763d7a9eed3c35bccad549acced261",
        "Toki Tori",
        "(USA, Europe) (En,Ja,Fr,De,Es)",
        "CGB-B2TE",
        168792713,
        1048576,
        "En,Ja,Fr,De,Es",
        "USA, Europe"
   , " " },
    {
        "98f7d6ec5287d9c4816feb70264366053c51591b",
        "Survival Kids 2 - Dasshutsu!! Futago-Jima!",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-B2VJ",
        2878023128,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "15485952a55e34860383c8e929f9985e9616018e",
        "Mega Man Xtreme 2",
        "(USA, Europe)",
        "CGB-B2XE",
        2414720728,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "a6c3a1035c6d297829e4c5947bde1917aaed556d",
        "Tweenies - Doodles' Bones",
        "(Europe) (En,De,Es,It)",
        "CGB-B2ZP",
        2466704848,
        1048576,
        "En,De,Es,It",
        "Europe"
   , " " },
    {
        "fe3a870569e96d3df3aabd8935a4009e7d67b9a3",
        "Tweenies - Doodles' Bones",
        "(Europe) (En,Nl,Sv,No,Da)",
        "CGB-B2ZX",
        3350598176,
        1048576,
        "En,Nl,Sv,No,Da",
        "Europe"
   , " " },
    {
        "b035b738efdfc4a16695ffa7eaf7fa8d80d18eda",
        "Medarot 3 - Kabuto Version",
        "(Japan)",
        "CGB-B32J",
        3864355628,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "9908140f71c46d8fec6cc918dfad6adcc8cb4864",
        "Medarot 3 - Kuwagata Version",
        "(Japan)",
        "CGB-B33J",
        3160504372,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "1009bf9f42b71251410c487159e9be13400b2242",
        "Jagainu-kun",
        "(Japan) (GB Comp.)",
        "DMG-B39J",
        2931176645,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "6dd8724f406024b45b2adb5fc30b9c688251b551",
        "Nakayoshi Cooking Series 5 - Cake o Tsukurou",
        "(Japan)",
        "CGB-B3CJ",
        3225762296,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "92b7b5970b3b67d7395f2919ef88a69e2e87bd22",
        "Nakayoshi Cooking Series 4 - Tanoshii Dessert",
        "(Japan)",
        "CGB-B3DJ",
        2119992642,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "fec95ee951ae685d82153c51c738ca4be38de458",
        "Beatmania GB - Gotcha Mix 2",
        "(Japan)",
        "CGB-B3GJ",
        3106565398,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "34dea1301db9c9008add3f3c7717558c0a8e94b7",
        "ESPN International Track & Field",
        "(USA)",
        "CGB-B3HE",
        1074124097,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "08aecf66c4164808a597826aa1c895d0e84d2406",
        "Ganbare! Nippon! Olympic 2000",
        "(Japan)",
        "CGB-B3HJ",
        4292417959,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "87046db0a5fb65a22ee0a825f0e8f26f780e0f8c",
        "International Track & Field - Summer Games",
        "(Europe)",
        "CGB-B3HP",
        3626420063,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "902867e3ef4a92baa038a647d0b3dd0aefc61a9d",
        "Battle of Kingdom",
        "(Japan)",
        "DMG-B3J",
        898715115,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "33196bb4b2884e82ae449e9fc443504f9ffe1295",
        "Santa Claus Junior",
        "(Europe)",
        "CGB-B3JP",
        2806308708,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "4f783c0fc740decb612e36bee85eb809ff2a1005",
        "Micro Maniacs",
        "(Europe)",
        "CGB-B3NP",
        3865971003,
        2097152,
        "En",
        "Europe"
   , " " },
    {
        "056510fa1567e7a11c21050a553e03ddd78afee2",
        "SnowCross",
        "(Europe) (En,Fr,De,Es,It,Pt)",
        "CGB-B3OP",
        1269071292,
        1048576,
        "En,Fr,De,Es,It,Pt",
        "Europe"
   , " " },
    {
        "7c3691755811e56b104e7b321e28fdbfd5c6cdde",
        "Medarot 3 - Parts Collection - Z kara no Chousenjou",
        "(Japan)",
        "CGB-B3PJ",
        3995965311,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "bbd790ad2e8bb6bca72140c1e9a4bcb1d55eb7b3",
        "Uchuujin Tanaka Tarou de - RPG Tsukuru GB 2",
        "(Japan)",
        "CGB-B3QJ",
        564019939,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "4b53903a80e7bf62139a87edb281414f83339202",
        "Shaun Palmer's Pro Snowboarder",
        "(USA, Australia)",
        "CGB-B3SE",
        1889327763,
        2097152,
        "En",
        "USA, Australia"
   , " " },
    {
        "cb1be5f4994291c33857cf3fb6eae1f741134857",
        "Tony Hawk's Pro Skater 3",
        "(USA, Europe)",
        "CGB-B3TE",
        4250046625,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "83934f9ca9d233be97352591acab3ccd266d6255",
        "Beetlejuice",
        "(USA)",
        "DMG-B4E",
        861359819,
        131072,
        "En",
        "USA"
   , " " },
    {
        "15aafebadebff3622307b0510e9cde4927f08f15",
        "Medarot 4 - Kabuto Version",
        "(Japan)",
        "CGB-B4MJ",
        3247612776,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "556fed6fbcb2a1d42b524cb63287fdee36c04522",
        "Medarot 4 - Kuwagata Version",
        "(Japan)",
        "CGB-B4NJ",
        1781119448,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "323b7932a03b68d70cff564f7d90f786bf4f23d4",
        "Sabrina - The Animated Series - Spooked!",
        "(USA, Europe)",
        "CGB-B4PE",
        754221448,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "3038812373f2c07da5c70b50a1c43b3da430107d",
        "Monster AG, Die",
        "(Germany)",
        "CGB-B4QD",
        982909672,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "0bd0050cf59d8500e0025195f0175408a5489bc9",
        "Monsters, Inc.",
        "(USA, Europe)",
        "CGB-B4QE",
        406698801,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "4b320acff249140db2f97fd30b377594848eb2e5",
        "Monsters, Inc.",
        "(Europe) (Rev 1)",
        "CGB-B4QP",
        925271434,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "54634b20dedcece52a3576ef47d81de675ad973c",
        "Monsters, Inc.",
        "(Europe) (En,Es,Nl)",
        "CGB-B4QX",
        1675869108,
        1048576,
        "En,Es,Nl",
        "Europe"
   , " " },
    {
        "1658d51e8fc440983c0fa902f88d80919f7f418e",
        "Monsters, Inc.",
        "(Europe) (En,Fr,It)",
        "CGB-B4QY",
        1898791077,
        1048576,
        "En,Fr,It",
        "Europe"
   , " " },
    {
        "4737832c0262eb7c0d79704d84c0521fff5b945e",
        "Saban's Power Rangers - Time Force",
        "(USA, Europe)",
        "CGB-B4TE",
        400888899,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "3e0bc9a6bd152126ceb8cc51a711d0c875bc81a6",
        "Loppi Puzzle Magazine - Hirameku Puzzle Dai-2-gou",
        "(Japan) (Rev 1) (SGB Enhanced, GB Comp.) (NP)",
        "DMG-B52J",
        3578615862,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "46665a2d2fbd6548a52520899fd1bcf49db21bb2",
        "Loppi Puzzle Magazine - Hirameku Puzzle Dai-2-gou",
        "(Japan) (SGB Enhanced, GB Comp.) (NP)",
        "DMG-B52J",
        702649285,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "1eecc221dd45403bad6bde208f59b67984ccfa16",
        "Loppi Puzzle Magazine - Hirameku Puzzle Dai-3-gou",
        "(Japan) (Rev 1) (SGB Enhanced, GB Comp.) (NP)",
        "DMG-B53J",
        106576885,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "a83e3d288004ba698e6a5cffec5f1b9d76ba338c",
        "Loppi Puzzle Magazine - Hirameku Puzzle Dai-3-gou",
        "(Japan) (SGB Enhanced, GB Comp.) (NP)",
        "DMG-B53J",
        3766016504,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "ff834ce72ce139bcb542c1748ddc24250550ac9d",
        "Gonta no Okiraku Daibouken",
        "(Japan)",
        "CGB-B54J",
        3831405010,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "703541b9992cdd0243675db0ae02f4ae339df345",
        "Batman - Return of the Joker",
        "(Japan)",
        "DMG-B5A",
        3729103431,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "043cee8108ae4830f5771059fa45549dcd1901b2",
        "Snobow Champion",
        "(Japan)",
        "CGB-B5CJ",
        2221926955,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "02ff68a7db9d88be56b25388cb5065be51fb1fa3",
        "Batman - Return of the Joker",
        "(USA, Europe)",
        "DMG-B5E",
        1361361900,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "1804c00a36f8b765d8a0e484bfba930611e8f1b9",
        "Loppi Puzzle Magazine - Hirameku Puzzle Soukangou",
        "(Japan) (SGB Enhanced, GB Comp.) (NP)",
        "DMG-B5IJ",
        3223878898,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "d11d18fb088574bfae0046fbfd89a11d7208dbde",
        "Loppi Puzzle Magazine - Hirameku Puzzle Soukangou",
        "(Japan) (Rev 1) (SGB Enhanced, GB Comp.) (NP)",
        "DMG-B5IJ",
        645616211,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "1989c9dea3d26f9af082ade1fbd54aec94858ab5",
        "Medarot 5 - Susutake Mura no Tenkousei - Kabuto",
        "(Japan)",
        "CGB-B5MJ",
        2747364718,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "b25ab9a924eca29bc8b4be51ce49dacdc22216a5",
        "Medarot 5 - Susutake Mura no Tenkousei - Kuwagata",
        "(Japan)",
        "CGB-B5NJ",
        21005224,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "0129d7a3c58b6271e6d4cdab034c397e0e355aae",
        "Loppi Puzzle Magazine - Kangaeru Puzzle Dai-2-gou",
        "(Japan) (Rev 1) (SGB Enhanced, GB Comp.) (NP)",
        "DMG-B62J",
        665526663,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "646b3e63543827d607bc0ea748905bff17edb496",
        "Loppi Puzzle Magazine - Kangaeru Puzzle Dai-2-gou",
        "(Japan) (SGB Enhanced, GB Comp.) (NP)",
        "DMG-B62J",
        3562523756,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "56e1181b1847dac4b1feef4e65921f772cfd0faa",
        "Loppi Puzzle Magazine - Kangaeru Puzzle Dai-3-gou",
        "(Japan) (Rev 1) (SGB Enhanced, GB Comp.) (NP)",
        "DMG-B63J",
        1658057189,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "9189501debeaac8f98d97bc2e1987bd95a1492db",
        "Loppi Puzzle Magazine - Kangaeru Puzzle Dai-3-gou",
        "(Japan) (SGB Enhanced, GB Comp.) (NP)",
        "DMG-B63J",
        1056068915,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "cd9fc81f7c8d13e2b07922153fead8f529ced447",
        "Great Greed",
        "(USA)",
        "DMG-B6E",
        3301701297,
        262144,
        "En",
        "USA"
   , " " },
    {
        "5e764640197813052a3d67131a894a174b5fc866",
        "Loppi Puzzle Magazine - Kangaeru Puzzle Soukangou",
        "(Japan) (Rev 1) (SGB Enhanced, GB Comp.) (NP)",
        "DMG-B6IJ",
        2295225363,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "f300c60c9a8b93b0e0183f9a3059fa6ab9fa246b",
        "Loppi Puzzle Magazine - Kangaeru Puzzle Soukangou",
        "(Japan) (SGB Enhanced, GB Comp.) (NP)",
        "DMG-B6IJ",
        156237086,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "55fffb3dd26d89e4b278c9e059d38d07163bbe05",
        "Vitamina Oukoku Monogatari",
        "(Japan)",
        "DMG-B6J",
        2309467850,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "f491f361f13bff07c8cb25a9f315729df6e8bba9",
        "Kinniku Banzuke GB 2 - Mezase! Muscle Champion",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-B6KJ",
        1121882258,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "62d8f6dafda9425cea58b0ddadafe53485e5d45b",
        "Nakayoshi Cooking Series 3 - Tanoshii Obentou",
        "(Japan)",
        "CGB-B7BJ",
        3715524302,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "84b2b36db1c96812cbe397956b12627bc7fa5b60",
        "Nakayoshi Cooking Series 1 - Oishii Cake-ya-san",
        "(Japan)",
        "CGB-B7CJ",
        3855177370,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "5c98a79bd912b8945aa511f31e2358d88b542e90",
        "Snow White and the Seven Dwarfs",
        "(USA)",
        "CGB-B7DE",
        2379449652,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "1852bf81130d1b74d851fb67fd7db5a2dde5147f",
        "Snow White and the Seven Dwarfs",
        "(Europe) (En,Fr,De,Es,It,Nl,Sv,No,Da)",
        "CGB-B7DP",
        3385230449,
        1048576,
        "En,Fr,De,Es,It,Nl,Sv,No,Da",
        "Europe"
   , " " },
    {
        "4d1c05172cf62903c5433250d6322bd5a91bb032",
        "Nakayoshi Pet Series 1 - Kawaii Hamster",
        "(Japan) (GB Comp.)",
        "DMG-B7HJ",
        2563833008,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "5207862c6ea9d33d91498e610d174d5b59b60d0d",
        "Nakayoshi Pet Series 3 - Kawaii Koinu",
        "(Japan)",
        "CGB-B7IJ",
        4199454870,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "16a75d63c7d47b78753342286da97518e4d91f36",
        "Kinniku Banzuke GB 3 - Shinseiki Survival Retsuden!",
        "(Japan)",
        "CGB-B7KJ",
        3807782206,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "76ce8b1974c894ece0b9aced8e9f0daf02a88b52",
        "Nakayoshi Pet Series 4 - Kawaii Koneko",
        "(Japan)",
        "CGB-B7NJ",
        439886695,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "e8914f06b0434f1c72468a9872b341df7f5b5bc7",
        "Nakayoshi Cooking Series 2 - Oishii Panya-san",
        "(Japan)",
        "CGB-B7PJ",
        3120184548,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "58b2f004d223813d05b2b94c29f6caba362fb7ce",
        "Nakayoshi Pet Series 2 - Kawaii Usagi",
        "(Japan) (GB Comp.)",
        "DMG-B7UJ",
        2254476738,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "a24ec29a7fe5963ae5ead0fe8e0e9b156ef5cb1f",
        "Densha de Go! 2",
        "(Japan)",
        "CGB-B82J",
        3072721916,
        8388608,
        "Ja",
        "Japan"
   , " " },
    {
        "94306ca82a48018ec0b65351aba344b31f093999",
        "Hamtaro - Ham-Hams Unite!",
        "(USA)",
        "CGB-B86E",
        309399935,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "2bbcec08206a9ee85a87ee1c3c107ef2aaed37e7",
        "Tottoko Hamutarou 2 - Hamu-chan Zu Daishuugou Dechu",
        "(Japan)",
        "CGB-B86J",
        4059811716,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "75ffd75891abbf36241a974e07c5d6f01dd92f75",
        "Hamtaro - Ham-Hams Unite!",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-B86P",
        1170120091,
        4194304,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "6e5c544a36e37453c9dee233ca84286740a43ee6",
        "Hana Yori Dango - Another Love Story",
        "(Japan)",
        "CGB-B87J",
        1184825988,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "ad32bff217ac3099c268870227f8ad15ac958589",
        "Booby Boys",
        "(Japan)",
        "DMG-B8J",
        3968057526,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "58e965247f5c195f587a0178be8082080b3b56b3",
        "Hamster Monogatari GB + Magi Ham no Mahou Shoujo",
        "(Japan)",
        "CGB-B8MJ",
        2882902329,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "e59fdb0956d177a43f5ccd3a48cc9e651bb665a1",
        "Data-Navi Pro Yakyuu 2",
        "(Japan)",
        "CGB-B8QJ",
        2517048016,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "18a175e4f8ac63e6cc585c23d0c5e7d7b1ad89a0",
        "Dokidoki Densetsu - Mahoujin Guruguru",
        "(Japan)",
        "CGB-B96J",
        2212788762,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "152-19769",
        "102 Dalmatiner",
        "(Germany)",
        "CGB-B99D",
        1918514307,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "151-25978",
        "102 Dalmatians - Puppies to the Rescue",
        "(USA, Europe)",
        "CGB-B99E",
        1454912825,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "150-31304",
        "102 Dalmatiens a la Rescousse, Les",
        "(France)",
        "CGB-B99F",
        3896611240,
        1048576,
        "Fr",
        "France"
   , " " },
    {
        "5a140e7f802fa4140b2b057c471fad9269a42653",
        "102 Dalmatas - Cachorros Al Rescate",
        "(Spain)",
        "CGB-B99S",
        4061301000,
        1048576,
        "Es",
        "Spain"
   , " " },
    {
        "0402948409bd00db36c9bed9db2c4a33a17111ff",
        "Mobile Trainer",
        "(Japan)",
        "CGB-B9AJ",
        1915153104,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "838785102e6b802bd23e920fb1f9d6202e2973f2",
        "Scrabble",
        "(Europe)",
        "CGB-B9QP",
        2575718327,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "f31cd74caf4a3878431f5178fd9beb1b26e59c4d",
        "Shougi 3",
        "(Japan) (GB Comp.)",
        "DMG-B9SJ",
        1689549306,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "60b113d7baf900ad1cba2f131b37009193be797a",
        "Tomb Raider - Curse of the Sword",
        "(USA, Europe)",
        "CGB-B9TE",
        46203738,
        4194304,
        "En",
        "USA, Europe"
   , " " },
    {
        "865bc2c4258d6edf293d9f7387263ee420bb3122",
        "Bomb Jack",
        "(Europe)",
        "DMG-B9X",
        2614657374,
        32768,
        "En",
        "Europe"
   , " " },
    {
        "dfdf4023ff549ab452928ab43d2221036f9b9179",
        "Army Men 2",
        "(USA, Europe) (En,Fr,De)",
        "CGB-BA2E",
        577944266,
        1048576,
        "En,Fr,De",
        "USA, Europe"
   , " " },
    {
        "5b3840b7fe2422fc2b558cf781dcbfe14f360cb8",
        "Antz World Sportz",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BA7P",
        1741158492,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "7d00d3237cd5ca5bc539c1fbed2234cd84cd5ca7",
        "Batman - The Video Game",
        "(World)",
        "DMG-BAA",
        1816253389,
        131072,
        "En",
        "World"
   , " " },
    {
        "dde2968503c662552c99f9bd7ee79865a271ef30",
        "Alfred's Adventure",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BAAP",
        1872687454,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "249a6f87551986b8acebd1eb80a31dd7078aaeb6",
        "Atlantis - The Lost Empire",
        "(USA, Europe)",
        "CGB-BABE",
        3583300171,
        2097152,
        "En",
        "USA, Europe"
   , " " },
    {
        "d60a7ba365d00c5bf97ac4c10fccd06638e0d578",
        "Atlantis - The Lost Empire",
        "(Europe) (En,Es,It)",
        "CGB-BABX",
        687330787,
        2097152,
        "En,Es,It",
        "Europe"
   , " " },
    {
        "b2be41fc3acfb766b7cf2ba19113bfa492032270",
        "Atlantis - The Lost Empire",
        "(Europe) (Fr,De,Nl)",
        "CGB-BABY",
        1308960016,
        2097152,
        "Fr,De,Nl",
        "Europe"
   , " " },
    {
        "2bcc59c3b69b22c464f00416c70ef4cd3df412b3",
        "Aladdin",
        "(USA)",
        "CGB-BADE",
        2837364825,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "c07a8e95770b5901f232d9b7e52ef34f7b9ae713",
        "Aladdin",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BADP",
        1147499469,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "15021d9513ecc7b3da718bbed31d6b129c240e64",
        "Aliens - Thanatos Encounter",
        "(USA, Europe)",
        "CGB-BAEE",
        3200500299,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "b0b8dbf61664bd35d743035494dbd89750418680",
        "Tsuriiko!!",
        "(Japan)",
        "CGB-BAFJ",
        1865202973,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "5da841eda7b9965ac500a01023bc566127b2fd31",
        "Rugrats - Typisch Angelica",
        "(Germany)",
        "CGB-BAGD",
        40650644,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "6210768ee4b7f20acf4d7f452c37a09c61504bca",
        "Rugrats - Totally Angelica",
        "(USA, Europe)",
        "CGB-BAGE",
        4234253807,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "ff0408b36c9bd576c707441b9c2b6189961a122b",
        "Razmoket, Les - 100% Angelica",
        "(France)",
        "CGB-BAGF",
        493275945,
        1048576,
        "Fr",
        "France"
   , " " },
    {
        "e214c16eccde9ce3070e1c3233482f56ac8a41a9",
        "New Addams Family Series, The",
        "(Europe) (En,Fr,De,Es,It,Pt)",
        "CGB-BAIP",
        4223235712,
        2097152,
        "En,Fr,De,Es,It,Pt",
        "Europe"
   , " " },
    {
        "e26fbba3276606c17dac845083aaa272cbf11fe4",
        "Koto Battle - Tengai no Moribito",
        "(Japan)",
        "CGB-BAJJ",
        1125036513,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "6db9946f6a79e1afa3e1354e5e17ba6b7eee5399",
        "Blue's Clues - Blue's Alphabet Book",
        "(USA)",
        "CGB-BALE",
        1955402565,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "c1119651d2deee2a2799b5f433e588e65cd5802d",
        "Animorphs",
        "(USA)",
        "CGB-BAME",
        3035796428,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "877f4e3c6ad12290df67988b80c881f593d0afb2",
        "Animorphs",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BAMP",
        2144628129,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "ed357e0fe5f264e427c3499c63ef1f53b948ee5a",
        "Bust-A-Move Millennium",
        "(USA, Europe)",
        "CGB-BANE",
        4175039562,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "672c2574f568c42b5e10bd6df999106aee7dc7ba",
        "Asterix & Obelix Contre Cesar",
        "(Europe) (En,Fr,De,Es,Nl) (GB Comp.)",
        "DMG-BAOP",
        1896103286,
        1048576,
        "En,Fr,De,Es,Nl",
        "Europe"
   , " " },
    {
        "a5f0d960f50752c7cd9812195179825801205c6f",
        "Austin Powers - Oh, Behave!",
        "(USA)",
        "CGB-BAPE",
        3146324238,
        4194304,
        "En",
        "USA"
   , " " },
    {
        "4bff850868a229578c0fdabf2576bbffd2a6cbff",
        "Austin Powers - Oh, Behave!",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BAPX",
        3214538955,
        4194304,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "1a8f6f0774de7a631fac9fe2b33947a04a5701af",
        "Armada - FX Racers",
        "(USA)",
        "CGB-BARE",
        3646081963,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "b74a7433b178d04f3fa23f3570c3ca3ec78208a6",
        "All-Star Baseball 2001",
        "(USA)",
        "CGB-BASE",
        3159762022,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "ded83c3550ec808265a723af1c767728301157ca",
        "Army Men - Air Combat",
        "(USA, Europe) (En,Fr,De)",
        "CGB-BATE",
        2966544012,
        1048576,
        "En,Fr,De",
        "USA, Europe"
   , " " },
    {
        "f18f7a3ddd1fe7da78aa64d854e4fa62f9b5db34",
        "Network Boukenki Bugsite - Alpha Version",
        "(Japan)",
        "CGB-BAUJ",
        3596389281,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "4a97530fa6b3b528033a73c78cdca42192b60b50",
        "Portal Runner",
        "(USA)",
        "CGB-BAVE",
        2436547334,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "101-52878",
        "Action Man - Search for Base X",
        "(USA, Europe)",
        "CGB-BAXE",
        304499102,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "0dc25e8498de62ce44d2530b363f048dabb188fe",
        "Antz Racing",
        "(USA) (En,Fr,De,Es,It,Nl)",
        "CGB-BAZE",
        3755970752,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "USA"
   , " " },
    {
        "ffae2aa831dadff7b246c4eed61c8a531ca51dd3",
        "Antz Racing",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BAZP",
        4129663321,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "9d1afe3daa2ad40d52f638dbe4d07db48bc21882",
        "Men in Black 2 - The Series",
        "(USA) (En,Fr,De)",
        "CGB-BB2E",
        2338583407,
        1048576,
        "En,Fr,De",
        "USA"
   , " " },
    {
        "5337323f4334551ffeb11e2d038fcb46756910f1",
        "Men in Black 2 - The Series",
        "(Europe) (En,Fr,De)",
        "CGB-BB2P",
        3395770675,
        1048576,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "fed33c491bd97672ff88bcc5940f62d0cf47ac99",
        "Bakuten Shoot Beyblade",
        "(Japan)",
        "CGB-BB3J",
        3157290660,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "9639ae34832b3937ad313d445d8ba3b1c4819cf0",
        "Goukaku Boy Series - Shikakui Atama o Maruku Suru - Sansuu Battle Hen",
        "(Japan) (IE Institute)",
        "DMG-BB4J",
        3732209001,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "524a7c3d89a2a35fa27f5346fdebbfaa49b09476",
        "Benjamin Bluemchen - Ein verrueckter Tag im Zoo",
        "(Germany)",
        "CGB-BB5D",
        1368861077,
        2097152,
        "De",
        "Germany"
   , " " },
    {
        "24ac1fe944765543c9897097466effe2bd644278",
        "Gyouten Ningen Batseelor - Doctor Guy no Yabou",
        "(Japan)",
        "CGB-BB6J",
        2977808995,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "8f15911a8a99668c41d489996dbe40b5022defbb",
        "Jim Henson's Bear in the Big Blue House",
        "(USA) (En,Fr,De,Es,It,Nl)",
        "CGB-BB7E",
        2233239312,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "USA"
   , " " },
    {
        "80481990b87a6aa09f4812c41881b2703b7cc75d",
        "Battle Fishers",
        "(Japan)",
        "CGB-BBAJ",
        3382506437,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "60c65f325e906125c04c1f1289196a3d00e869bb",
        "Batman - Chaos in Gotham",
        "(USA)",
        "CGB-BBBE",
        2990646515,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "3a8e67f3e4f4ca39d5ed53ef27c8ca166b6b388d",
        "Batman - Chaos in Gotham",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BBBP",
        3760522875,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "a2b3c00fc3c6e3482bdbca3bfce814b8a43a03cd",
        "DT - Lords of Genomes",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-BBDJ",
        1120843860,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "5c374fc88cb3dcd45fecdae127301d1515abf504",
        "Bugs Bunny Crazy Castle, The",
        "(USA, Europe)",
        "DMG-BBE",
        1077812094,
        65536,
        "En",
        "USA, Europe"
   , " " },
    {
        "17f042db31c2633207d5c14c78d2ad459ea1ecba",
        "Trouballs",
        "(USA)",
        "CGB-BBEE",
        638512388,
        524288,
        "En",
        "USA"
   , " " },
    {
        "9b680bc909e3b03ebab3a42cd576ad0e6dd27480",
        "Powerpuff Girls, The - Battle Him",
        "(USA)",
        "CGB-BBHE",
        3628423556,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "f1bc089cd40672bfb04a276d0ba33758a645a7c3",
        "Powerpuff Girls, The - Battle Him",
        "(USA) (Rev 1)",
        "CGB-BBHE",
        3822491212,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "87e27b2760c5ce135d051f71feae455f15761d26",
        "Powerpuff Girls, The - Bulle Contre Lui",
        "(France)",
        "CGB-BBHF",
        1887774476,
        2097152,
        "Fr",
        "France"
   , " " },
    {
        "ca952d8e8038ba5d6c3b6e241bfafedc44d8d5ba",
        "Powerpuff Girls, The - Battle Him",
        "(Europe)",
        "CGB-BBHP",
        2022201774,
        2097152,
        "En",
        "Europe"
   , " " },
    {
        "17365968e2e0378951c16d5a84c68a7d2457dd74",
        "Super Nenas, Las - Lucha Con Ese",
        "(Spain)",
        "CGB-BBHS",
        2074639377,
        2097152,
        "Es",
        "Spain"
   , " " },
    {
        "c6eb531832892f76abda25deac71a7931e5e6351",
        "Ferret Monogatari",
        "(Japan)",
        "CGB-BBIJ",
        2339647413,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "41d3e12539c70e8dc1693ac62d4183b4634ac9b9",
        "Chi to Ase to Namida no Koukou Yakyuu",
        "(Japan)",
        "CGB-BBJJ",
        1010416811,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "7ae2a87edb875e38a1c6d688f99d9743de4a1637",
        "Balloon Fight GB",
        "(Japan) (SGB Enhanced, GB Comp.) (NP)",
        "DMG-BBKJ",
        3534709966,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "c5d38ba21ca9a432ef12ca693f1a682d1b720763",
        "Jim Henson's Bear in the Big Blue House",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BBNP",
        1221171513,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "6d37dd8f8ceca6e3d9943c827a3420468b5714d8",
        "Black Onyx, The",
        "(Japan)",
        "CGB-BBOJ",
        1479533368,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "d9c8f32377a593697c25f705af61789efca0b2cc",
        "Bomberman Max - Ain Version",
        "(Japan)",
        "CGB-BBPJ",
        1415450018,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "aed5a557a5cebcc929ffe57f8f65d42cdf193295",
        "Bob et Bobette - Les Dompteurs du Temps",
        "(Europe) (Fr,Nl)",
        "CGB-BBQP",
        2764460605,
        524288,
        "Fr,Nl",
        "Europe"
   , " " },
    {
        "de8f05ae50866ba6da6b6f06c536dd178c1f6d26",
        "Bad Badtz-Maru Robo Battle",
        "(Japan)",
        "CGB-BBRJ",
        1305647881,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "0d3dc5daa875eb5e98ccc0451420cb1fe626142f",
        "Brave Saga - Shinshou Astaria",
        "(Japan)",
        "CGB-BBSJ",
        1566386506,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "f6a9eb8d1c87add3b828cd0bcb4bde57bd85a6a1",
        "WDL - World Destruction League - Thunder Tanks",
        "(USA) (En,Fr,De)",
        "CGB-BBTE",
        436991191,
        1048576,
        "En,Fr,De",
        "USA"
   , " " },
    {
        "3c19ef818c0a85c225effb69061593471cd51341",
        "Network Boukenki Bugsite - Beta Version",
        "(Japan)",
        "CGB-BBUJ",
        1458906723,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "ab36cae2a62cb6591edfcbe0fb04c99fea646441",
        "Beyblade - Fighting Tournament",
        "(Japan)",
        "CGB-BBVJ",
        516545429,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "1a12b03ee7106ea0a35829ceb9d8db753d8dc2f8",
        "Kikansha Thomas - Sodor-tou no Nakama-tachi",
        "(Japan)",
        "CGB-BBWJ",
        574337436,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "76a2a91c8dcb11c331bd5e852bbb785e0530eae4",
        "Yogi Bear - Great Balloon Blast",
        "(USA)",
        "CGB-BBXE",
        4162316664,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "ee7bca49300318d17a59a5f92a08495974a48d86",
        "Dragon Ball Z - Legendaere Superkaempfer",
        "(Germany)",
        "CGB-BBZD",
        3967679937,
        2097152,
        "De",
        "Germany"
   , " " },
    {
        "86b536a003f007d551894fa6ed47407ae8466bff",
        "Dragon Ball Z - Legendary Super Warriors",
        "(USA)",
        "CGB-BBZE",
        39495818,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "b91d2920d41bde0cd4330849fc82009d0198d524",
        "Dragon Ball Z - Les Guerriers Legendaires",
        "(France)",
        "CGB-BBZF",
        3229520952,
        2097152,
        "Fr",
        "France"
   , " " },
    {
        "1a5aa547393bdbab316c002a495472c534476201",
        "Dragon Ball Z - I Leggendari Super Guerrieri",
        "(Italy)",
        "CGB-BBZI",
        2827526217,
        2097152,
        "It",
        "Italy"
   , " " },
    {
        "665a7a4fbb686f390b156481c8079c4cc2310f8e",
        "Dragon Ball Z - Densetsu no Chou Senshi-tachi",
        "(Japan)",
        "CGB-BBZJ",
        2099800218,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "6c3b84f500dc3d6e6ca5a98e8aca718864d938f3",
        "Dragon Ball Z - Legendary Super Warriors",
        "(Europe)",
        "CGB-BBZP",
        2079864539,
        2097152,
        "En",
        "Europe"
   , " " },
    {
        "c940f4b5b436da611a7eeec98845bbb5092bf0f6",
        "Dragon Ball Z - Guerreros de Leyenda",
        "(Spain)",
        "CGB-BBZS",
        2645388835,
        2097152,
        "Es",
        "Spain"
   , " " },
    {
        "3533b6a0f0e21c855b61f3d20e8290878a48ef8d",
        "Bikkuriman 2000 - Charging Card GB",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-BC2J",
        1541558652,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "20b782acbd19312b95693ab06597c629d009043a",
        "Meitantei Conan - Norowareta Kouro",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-BC3J",
        3401439234,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "3cc7e0d061aecd6f23856412009526c6e7edee2d",
        "Goukaku Boy GOLD - Shikakui Atama o Maruku Suru - Keisan no Tatsujin",
        "(Japan) (Imagineer)",
        "DMG-BC4J",
        1907145575,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "f8caf2711e43b8b436ef886f310b9445d457b5ea",
        "Goukaku Boy GOLD - Shikakui Atama o Maruku Suru - Keisan no Tatsujin",
        "(Japan) (IE Institute)",
        "DMG-BC6J",
        69291922,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "8d4808324792337cb5aa60c553c12477a47166ce",
        "Cross Hunter - Treasure Hunter Version",
        "(Japan)",
        "CGB-BC7J",
        1648104726,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "bcefe0f3ce507640bf60308c2483f639f425dd68",
        "Cross Hunter - Monster Hunter Version",
        "(Japan)",
        "CGB-BC8J",
        1222752893,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "b57e895328f121e552123123a483c210f62369c6",
        "Cross Hunter - X Hunter Version",
        "(Japan)",
        "CGB-BC9J",
        2239762656,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "7355143516c3c54eb7e5de754acde12f3922783b",
        "BattleCity",
        "(Japan)",
        "DMG-BCA",
        2742714698,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "f870d052f0878212c03f13706931840994066e3f",
        "Commander Keen",
        "(USA, Europe)",
        "CGB-BCBE",
        1257557148,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "d2911d2e47422470ed74c223131ca917237f5a62",
        "Scooby-Doo! - Classic Creep Capers",
        "(USA, Europe)",
        "CGB-BCCE",
        3815786325,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "269ebe8744471eb4ae466cdbe0127854657ceed9",
        "Cannon Fodder",
        "(USA) (En,Fr,De,Es,It)",
        "CGB-BCFE",
        653844896,
        4194304,
        "En,Fr,De,Es,It",
        "USA"
   , " " },
    {
        "b664008ad14409c234fd1f464266157d083582f5",
        "Cannon Fodder",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BCFP",
        2186034163,
        4194304,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "b1b90ae8587898117fb471238412f11061c71815",
        "Hoyle Card Games",
        "(USA)",
        "CGB-BCGE",
        3713873683,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "9e9ccbe667a905afc61899b1f80c1eeb636b9100",
        "Chicken Run",
        "(USA, Europe) (En,Fr,De,Es,It)",
        "CGB-BCKE",
        4173146625,
        1048576,
        "En,Fr,De,Es,It",
        "USA, Europe"
   , " " },
    {
        "37601a725786cb6a5d519ee58be379666c0d97c0",
        "Carl Lewis Athletics 2000",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BCLP",
        356714873,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "a7a8e76e06dc298ff6eaf431354bb50def740e56",
        "Championship Motocross 2001 featuring Ricky Carmichael",
        "(USA, Europe)",
        "CGB-BCME",
        3417519853,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "f4ede87b0737f49ecf0dbac4b2ecf52fe18ec795",
        "Chibi Maruko-chan - Go Chounai Minna de Game Da yo!",
        "(Japan)",
        "CGB-BCNJ",
        553296081,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "6ff238444b681854401a9d257159bccc096439d1",
        "Colin McRae Rally",
        "(Europe)",
        "CGB-BCOP",
        3912956360,
        2097152,
        "En",
        "Europe"
   , " " },
    {
        "2b27b954bb07beed365423fe8faefa36c5721563",
        "Roswell Conspiracies - Aliens, Myths & Legends",
        "(USA) (En,Fr,De)",
        "CGB-BCPE",
        526303537,
        1048576,
        "En,Fr,De",
        "USA"
   , " " },
    {
        "8b803fae0a06173c29d50b549f2bd307c8deb299",
        "Roswell Conspiracies - Aliens, Myths & Legends",
        "(Europe) (En,Fr,De)",
        "CGB-BCPP",
        1225790806,
        1048576,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "2ce99409598d75480c2f5995e300c8ed901699f0",
        "Itsudemo Pachinko GB - CR Monster House",
        "(Japan)",
        "CGB-BCRJ",
        2202046906,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "fdd379afa8f103e1072629bc99c15e797f2848ca",
        "Hoyle Casino",
        "(USA)",
        "CGB-BCSE",
        1093956528,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "46220ee908c84c74d06d0645319074b64f869609",
        "CyberTiger",
        "(USA, Europe)",
        "CGB-BCTE",
        3832597242,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "d0f7cbb7558dc957a7c3d848d4b0fd2402fb41d1",
        "Hamster Paradise 4",
        "(Japan)",
        "CGB-BCUJ",
        3294682248,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "a39a18b9e483b4d756171d14cc7c3adedc7e6001",
        "Mary-Kate and Ashley - Crush Course",
        "(USA, Europe)",
        "CGB-BCVE",
        1775975640,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "cd9f3fd286e63073ffea4f41210a20fa47799a7c",
        "Cubix - Robots for Everyone - Race 'n Robots",
        "(USA) (En,Fr,De,Es,It)",
        "CGB-BCXE",
        2676505359,
        1048576,
        "En,Fr,De,Es,It",
        "USA"
   , " " },
    {
        "f134ca77a63c269bb800497e9eac067485d44b68",
        "Tonka Construction Site",
        "(USA)",
        "CGB-BCZE",
        2168628200,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "3fa6681b5cca39f4e1e532458dce0cbf0dc444a6",
        "Dance Dance Revolution GB 2",
        "(Japan)",
        "CGB-BD2J",
        1449126966,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "71ef4cb38863593757647b5d1f652cc4f331b4eb",
        "Dragon Warrior III",
        "(USA)",
        "CGB-BD3E",
        265930140,
        4194304,
        "En",
        "USA"
   , " " },
    {
        "cdc462aea445a5e7c512512d5a32df0339071dc3",
        "Dragon Quest III - Soshite Densetsu e...",
        "(Japan)",
        "CGB-BD3J",
        554142742,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "6014915d72c026b1123a50176db09857d0b946c6",
        "Goukaku Boy GOLD - Shikakui Atama o Maruku Suru - Nanmon no Sho",
        "(Japan) (Imagineer)",
        "DMG-BD4J",
        1016102700,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "6bf4b3b90bdb092acb1da43c0e06d536df39b594",
        "Ohasuta Dance Dance Revolution GB",
        "(Japan)",
        "CGB-BD5J",
        3812147480,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "fef6106082da04152b5d1b0473581eb8e3f08468",
        "Dance Dance Revolution GB 3",
        "(Japan)",
        "CGB-BD6J",
        1492283190,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "d948a2e9b0a4e79c65ed32742aba78ccc4d44dad",
        "Dance Dance Revolution GB - Disney Mix",
        "(Japan)",
        "CGB-BD7J",
        3004848575,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "483ec8df72e97448d086bcbcf7936148b69af226",
        "Diva Starz",
        "(Germany)",
        "CGB-BD8D",
        1443454854,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "76fc55d550743f514b2254e93fa7153e7a0b9945",
        "Diva Starz - Mall Mania",
        "(USA)",
        "CGB-BD8E",
        3957386454,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "fde9bbec2f4231fc35ff8f98f81547ad956b2ed7",
        "Diva Starz",
        "(France)",
        "CGB-BD8F",
        4229641937,
        1048576,
        "Fr",
        "France"
   , " " },
    {
        "70065fb600d372c1f3d73a93db7da96e3f7458dd",
        "Diva Starz",
        "(Europe)",
        "CGB-BD8P",
        1148643231,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "c0d2a99bc1cf3ef20ec854a603c3271861d5eb75",
        "Denki Blocks!",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BD9P",
        955255156,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "171763a40d24c0a390ff63d8d0da07fe70a5db90",
        "Boulder Dash",
        "(Japan)",
        "DMG-BDA",
        3048470619,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "939985a709f4e6c04d54d6cc48bb45653f60d2e5",
        "Dragon Dance",
        "(USA) (SGB) (GB Comp.)",
        "DMG-BDAE",
        100850657,
        262144,
        "En",
        "USA"
   , " " },
    {
        "25d7300eb066a4dc40a3b909053eb846aef78eb2",
        "Bakusou Dekotora Densetsu GB Special - Otoko Dokyou no Tenka Touitsu",
        "(Japan)",
        "CGB-BDBJ",
        2462628277,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "8d2c42fbd360a1fc8027a5a842c4305a6977d528",
        "Dukes of Hazzard, The - Racing for Home",
        "(USA)",
        "CGB-BDCE",
        4211662059,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "e2d4c5564cc0cfe13691dc1dfa3f8046bb5c0dd5",
        "Dukes of Hazzard, The - Racing for Home",
        "(Europe) (En,Fr,De)",
        "CGB-BDCP",
        562195164,
        2097152,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "bec7f56b536f589875888473f47d818b4e9213dc",
        "Donkey Kong Country",
        "(USA, Europe) (En,Fr,De,Es,It)",
        "CGB-BDDE",
        2977182839,
        4194304,
        "En,Fr,De,Es,It",
        "USA, Europe"
   , " " },
    {
        "1659582581702db31b3043fe71b43f26ad8b95ec",
        "Donkey Kong 2001",
        "(Japan)",
        "CGB-BDDJ",
        3406192314,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "c63ed99d46072acbc198dbc6bddd585a92ca3b6e",
        "Boulder Dash",
        "(Europe)",
        "DMG-BDE",
        1682631742,
        65536,
        "En",
        "Europe"
   , " " },
    {
        "19422098434cbd5effc8e99a02dd074e9b35e9ae",
        "Moorhuhn 2 - Die Jagd geht weiter",
        "(Germany)",
        "CGB-BDED",
        3981627055,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "d790c27919ea0650ddeffab7588be0f681e33ac3",
        "Dr. Rin ni Kiitemite! - Koi no Rin Fuusui",
        "(Japan)",
        "CGB-BDFJ",
        3960900193,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "2b56519ff4a08db8fac6cada5b1997f8eb0c9a82",
        "Dance Dance Revolution GB",
        "(Japan)",
        "CGB-BDGJ",
        3327128246,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "f13233d5136c21d320dd857b4f285d8a52da5205",
        "Tanimura Hitoshi Ryuu Pachinko Kouryaku Daisakusen - Don Quijote ga Iku",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-BDHJ",
        3465209228,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "daf39fa84a80f60227e84a26173d09ee1fc0fb64",
        "Gekisou Dangun Racer - Onsoku Buster Dangun Dan",
        "(Japan)",
        "CGB-BDJJ",
        117231356,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "d0ad1d68572428104a4df2db01594db277dd8219",
        "Dokapon! - Millennium Quest",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-BDKJ",
        1340729702,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "186-12031",
        "AirForce Delta",
        "(USA)",
        "CGB-BDLE",
        4281453714,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "182-46252",
        "AirForce Delta",
        "(Japan)",
        "CGB-BDLJ",
        1577055714,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "971504924b6fdea5bba2240b1e85043c19e4b017",
        "Deadly Skies",
        "(Europe) (En,Fr,De)",
        "CGB-BDLP",
        1076749176,
        1048576,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "f695b6d4da7212bc0d340c1003fee9464dc39271",
        "Doraemon Memories - Nobita no Omoide Daibouken",
        "(Japan) (GB Comp.)",
        "DMG-BDMJ",
        3515338436,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "68fd73f8cc4055182f4104ffc7fe24761e95e7e4",
        "Dinosaur",
        "(USA)",
        "CGB-BDNE",
        661572270,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "19cc6ac49c81812c8bc4d5223c09626d88c86f6b",
        "Dinosaur",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BDNP",
        178835660,
        2097152,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "1fb267b3c17b8ed7fd9370205dca54a271887803",
        "Daa! Daa! Daa! - Totsuzen Card de Battle de Uranai de!",
        "(Japan)",
        "CGB-BDOJ",
        4095456146,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "cb168bbc46bd1a84ab020deab2a9441e8ad68b31",
        "Boku no Camp-jou",
        "(Japan)",
        "CGB-BDPJ",
        3362857141,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "5f0bfcbdb4349eb3541f3690bfc0ce13d303e270",
        "Tiny Toon Adventures - Dizzy's Candy Quest",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BDQP",
        1510818218,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "c4a300fedf822bf095e287850f03ed1c37a3c282",
        "Driver - You Are the Wheelman",
        "(USA) (En,Fr,De,Es,It)",
        "CGB-BDRE",
        3569874655,
        1048576,
        "En,Fr,De,Es,It",
        "USA"
   , " " },
    {
        "7016272ea4694dab4ad316d56f9c39125c4f8de4",
        "Driver",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BDRP",
        3924194307,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "72032a4208f893010a4dca8ad61fefefe2076c79",
        "Dinosaur'us",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BDSE",
        4099973091,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "6b9925656c105c4d557c28f3f7b326d11bda4b37",
        "Kaitei Densetsu!! Treasure World",
        "(Japan) (GB Comp.)",
        "DMG-BDTJ",
        1282562265,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "9900eb8d4ee743470f640a930c901a573ea5f65b",
        "Doug's Big Game",
        "(Germany)",
        "CGB-BDUD",
        1573016785,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "23e4b5da364986ef5188b2c8bb777e7a364478f7",
        "Doug's Big Game",
        "(USA)",
        "CGB-BDUE",
        149147749,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "6b190374738a3389ff2a5827c9519bf633c9e8f6",
        "Doug - La Grande Aventure",
        "(France)",
        "CGB-BDUF",
        2850434842,
        1048576,
        "Fr",
        "France"
   , " " },
    {
        "3cf44af9461f06b7f70d8053aac702d417d07634",
        "Doug's Big Game",
        "(Europe)",
        "CGB-BDUP",
        3070213322,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "d6a017907bc802a06a52e3d2e9d2f061c598e942",
        "Bouken! Dondoko Shima",
        "(Japan)",
        "CGB-BDVJ",
        1608997319,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "9e04c3926e3d90dc8793955faac876fc55729e13",
        "Dragon Tales - Dragon Adventures",
        "(USA)",
        "CGB-BDWE",
        3728279049,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "3bf7903ecf358dacf0fd49ba24a8bdba74bb711e",
        "Dragon's Lair",
        "(USA, Europe) (En,Ja,Fr,De,Es,Zh)",
        "CGB-BDXE",
        3204934821,
        4194304,
        "En,Ja,Fr,De,Es,Zh",
        "USA, Europe"
   , " " },
    {
        "ed8709500fc0a25c425dae3aac4bddf1b9f786bc",
        "Donkey Kong Country",
        "(USA) (En,Fr,De,Es,It) (Demo) (Kiosk)",
        "CGB-BDYE",
        134612387,
        4194304,
        "En,Fr,De,Es,It",
        "USA"
   , " " },
    {
        "a1d5c99520d695d205551ef5ee422a04099ca732",
        "Dragon Tales - Dragon Wings",
        "(USA)",
        "CGB-BDZE",
        421343724,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "97d81fa288d3879213972105a661b93b9d31c48f",
        "Dragon Tales - Dragon Wings",
        "(Europe)",
        "CGB-BDZP",
        326990661,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "f7610c2e7cc8c4cddc0a381265243f0066f05cc9",
        "Goukaku Boy Series - Shikakui Atama o Maruku Suru - Shakai Battle Hen",
        "(Japan) (IE Institute)",
        "DMG-BE3J",
        2342923069,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "6aa647645ca57dcd11c9bb29bc8c4e419dc737b7",
        "Austin Powers - Welcome to My Underground Lair!",
        "(USA)",
        "CGB-BEAE",
        2879662968,
        4194304,
        "En",
        "USA"
   , " " },
    {
        "0d8b99597a343ea1399bd157560050e2fbe9926a",
        "Austin Powers - Welcome to My Underground Lair!",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BEAX",
        1765757146,
        4194304,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "bcc8321f3835d16525e4ffdf0e91a4d9a2f7ced0",
        "Extreme Ghostbusters",
        "(Europe) (En,Fr,De,Es,It,Pt)",
        "CGB-BEBP",
        3430382488,
        1048576,
        "En,Fr,De,Es,It,Pt",
        "Europe"
   , " " },
    {
        "1675f4f854310a8f8a34fc480c17284e7c977c76",
        "ECW Hardcore Revolution",
        "(USA, Europe)",
        "CGB-BECE",
        1213118992,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "f5c4bd6c2f6907e255a3cd9c9bc6ce7b807ffd19",
        "Gold and Glory - The Road to El Dorado",
        "(USA)",
        "CGB-BEDE",
        3084162192,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "f9752c99326d41c1f241c0e99e00abf5184e5e41",
        "Gold and Glory - The Road to El Dorado",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BEDP",
        2976847698,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "afbebf8e16e8b2bf3c476e4c1b4f9a5f10e37882",
        "Ganbare Goemon - Seikuushi Dynamites Arawaru!!",
        "(Japan)",
        "CGB-BEKJ",
        782817748,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "e3479a92d44640e451700bbf9f37c26195b575c8",
        "Raku x Raku - Cut Shuu",
        "(Japan)",
        "CGB-BELJ",
        1124924659,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "04da5aa3468878629115d286d94bfcc43208dcfa",
        "Raku x Raku - Moji",
        "(Japan)",
        "CGB-BEMJ",
        2883947147,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "60b504421df74b98642adec92e31eaca88fbee2e",
        "Emperor's New Groove, The",
        "(USA)",
        "CGB-BENE",
        1857738041,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "6f1910dc56df06cb1dc6e3e9898002508208fa13",
        "Emperor's New Groove, The",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BENP",
        3945383300,
        2097152,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "1b00689ac759f629ca73228c25c9c44f5bc9bc31",
        "E.T. - The Extra-Terrestrial and the Cosmic Garden",
        "(USA)",
        "CGB-BEOE",
        851999064,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "346a01de102a4df7583f95331d0e08697fae96bd",
        "E.T. - The Extra-Terrestrial and the Cosmic Garden",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BEOP",
        2200019385,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "5d5563b0f0328a46154daea882e0228929242133",
        "Matchbox Emergency Patrol",
        "(USA, Europe)",
        "CGB-BEPE",
        2750561662,
        2097152,
        "En",
        "USA, Europe"
   , " " },
    {
        "c7a8240d99181b10670a750c6d1295f24bc8d28c",
        "E.T. - The Extra-Terrestrial - Escape from Planet Earth",
        "(USA)",
        "DMG-BERE",
        140576478,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "893048a973f7a269bd9051e3c806213a4fffbea8",
        "E.T. - The Extra-Terrestrial - Escape from Planet Earth",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BERP",
        2992586165,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "f4179e88ff769b438fa36e0cf86a1c109668c943",
        "Extreme Sports with the Berenstain Bears",
        "(USA, Europe) (En,Fr,De,Es,It)",
        "CGB-BESE",
        1544452932,
        1048576,
        "En,Fr,De,Es,It",
        "USA, Europe"
   , " " },
    {
        "d5392ec74327983b8b645c86750959a9581b7f34",
        "E.T. - The Extra-Terrestrial - Digital Companion",
        "(USA)",
        "CGB-BETE",
        2223450521,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "42fc6f5923277ba5d4e04c8514ce90174fd5e9aa",
        "Elevator Action EX",
        "(Japan)",
        "CGB-BEXJ",
        3071036891,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "3454f39a74fee275299f450080dd5852d4af1a0f",
        "Elevator Action EX",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BEXP",
        559183420,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "23b201ec797a37d0069b44463d27caeae99b689d",
        "F1 World Grand Prix II for Game Boy Color",
        "(USA) (En,Fr,De,Es)",
        "CGB-BF2E",
        1210978501,
        1048576,
        "En,Fr,De,Es",
        "USA"
   , " " },
    {
        "d5fac64bbd86f4f12ce90e0db2c8c8443b9d7599",
        "F1 World Grand Prix II for Game Boy Color",
        "(Japan) (En,Ja)",
        "CGB-BF2J",
        675624039,
        1048576,
        "En,Ja",
        "Japan"
   , " " },
    {
        "67c40eb07f0ea41bde958804e4e447a717400bea",
        "F1 World Grand Prix II for Game Boy Color",
        "(Europe) (En,Fr,De,Es)",
        "CGB-BF2P",
        4009009675,
        1048576,
        "En,Fr,De,Es",
        "Europe"
   , " " },
    {
        "cb0bededeb695a48da0e0fa6b0e4da0a2f0f0b60",
        "Goukaku Boy Series - Shikakui Atama o Maruku Suru - Kokugo Battle Hen",
        "(Japan) (IE Institute)",
        "DMG-BF3J",
        2121144021,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "f3d62673701d1cc7e42c1934605bcecd11998a7a",
        "Anpfiff - Der RTL Fussball-Manager",
        "(Germany)",
        "CGB-BFBD",
        3041732170,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "4fa68a21f9a362533e250280317f57d6871f6ade",
        "Dancing Furby",
        "(Japan) (GB Comp.)",
        "DMG-BFBJ",
        845411986,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "1303e0e21bce6418e4b04a839a8080452257bb99",
        "Player Manager 2001",
        "(Europe) (En,Fr)",
        "CGB-BFBP",
        928789984,
        1048576,
        "En,Fr",
        "Europe"
   , " " },
    {
        "36f83e4251262382174cf98de2ae9652c720cb4e",
        "Force 21",
        "(USA) (En,Fr,De)",
        "CGB-BFCE",
        768412170,
        1048576,
        "En,Fr,De",
        "USA"
   , " " },
    {
        "20460711afdfd745d89bc2d5297bfe55acb91e58",
        "Arthur's Absolutely Fun Day!",
        "(USA)",
        "CGB-BFDE",
        4030044579,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "49ca019fcff6dce8cb202eea6bd45eb89f8f7001",
        "Bibi und Tina - Fohlen Felix in Gefahr",
        "(Germany)",
        "CGB-BFED",
        2289347796,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "192196df26f20f470118e6b3a6711f6236c6dbb3",
        "Fix & Foxi - Episode 1 - Lupo",
        "(Europe) (En,Fr,De)",
        "CGB-BFFP",
        1576821224,
        1048576,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "ae55dc93705dd1344ce520567fe47bf6a675124d",
        "Gobs of Games",
        "(USA) (En,Fr,De)",
        "CGB-BFGE",
        778158993,
        1048576,
        "En,Fr,De",
        "USA"
   , " " },
    {
        "962bf255ea953863ff58e7f61c61a8321abc8462",
        "Games Frenzy",
        "(Europe) (En,Fr,De)",
        "CGB-BFGP",
        814369995,
        1048576,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "48dfb79f483dc2633f86a0588dee0391792153a5",
        "Baby Felix - Halloween",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BFHP",
        2112707697,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "0b74e2066c5997ec7461802c53d8ae324cd6af49",
        "F1 Championship Season 2000",
        "(Brazil) (En,Fr,De,Es,It)",
        "CGB-BFIB",
        4109757720,
        2097152,
        "En,Fr,De,Es,It",
        "Brazil"
   , " " },
    {
        "c57432c030ec738162bcfa83cd19a4dfd2ef9801",
        "F1 Championship Season 2000",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BFIP",
        1544565086,
        2097152,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "06a67313e7f5cfc104289454a2e462e8b4ba335d",
        "Flipper & Lopaka",
        "(Europe) (En,Fr,De,Es,It,Nl,Pt,Sv,No,Da)",
        "CGB-BFLP",
        146400426,
        1048576,
        "En,Fr,De,Es,It,Nl,Pt,Sv,No,Da",
        "Europe"
   , " " },
    {
        "c69e7eb4c0f3a17d470e647e33a52e3cb42bf0b3",
        "Formula One 2000",
        "(USA)",
        "CGB-BFME",
        1882981759,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "d84447553687c7fc126bd6459871cf8e66f026c7",
        "Barbie - Fashion Pack Games",
        "(USA, Europe) (GB Comp.)",
        "DMG-BFPE",
        166630312,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "4f571dd196c1e7ea841c52007b0365482c199f4e",
        "Fish Files, The",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BFQP",
        3601580536,
        4194304,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "debe1d79eaafd4f7fcc957482ad2fdfea62b1f57",
        "Front Line - The Next Mission",
        "(Japan)",
        "CGB-BFRJ",
        4102952515,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "fe97f217162c2664c4af6455b17cb53acaf5c202",
        "Flintstones, The - Burgertime in Bedrock",
        "(USA)",
        "CGB-BFSE",
        349752413,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "e3660b7e0b46dfab605673c4d6e01f3c85bffed0",
        "Flintstones, The - Burgertime in Bedrock",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BFSP",
        2779813670,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "633c7a08311894a91ace0fd863e13a2322772780",
        "Wizardry Empire - Fukkatsu no Tsue",
        "(Japan)",
        "CGB-BFTJ",
        437277995,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "f02a06b10e4700313c0258984eb6f46b7c9d96e2",
        "Shrek - Fairy Tale Freakdown",
        "(USA, Europe) (En,Fr,De,Es,It)",
        "CGB-BFUE",
        947807321,
        2097152,
        "En,Fr,De,Es,It",
        "USA, Europe"
   , " " },
    {
        "0f82095611246fd873f8ee8bef5f61fa84831b5e",
        "Monster Traveler",
        "(Japan) (Rev 1)",
        "CGB-BFVJ",
        4127864686,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "a948487a6b718540b5d127fd34580af6ba97f5a1",
        "Fushigi no Dungeon - Fuurai no Shiren GB 2 - Sabaku no Majou",
        "(Japan) (BFWJ)",
        "CGB-BFWJ",
        4089581502,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "6ab7310c05eac883809f5bb2c575ef3dbb71cc06",
        "Titus the Fox",
        "(USA) (GB Comp.)",
        "DMG-BFXE",
        3127956111,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "c0b3d11d3461fbffe8c6dc7b433e73bb331788f9",
        "Titus the Fox",
        "(Europe) (GB Comp.)",
        "DMG-BFXP",
        798955998,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "46745dd45c674779284431fe8acabd5e419dc62c",
        "Street Fighter Alpha - Warriors' Dreams",
        "(Japan)",
        "CGB-BFZJ",
        846437172,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "09f07c334b0eb7e5e2e20b1c02a7b9d9635cb115",
        "Game Conveni 21",
        "(Japan) (GB Comp.)",
        "DMG-BG2J",
        2571310259,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "b8f326aef34b20a8cb1f06e8ad80726d1f28b019",
        "Grand Theft Auto 2",
        "(USA)",
        "CGB-BGAE",
        1751187971,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "c80359bdaa4dc6427a5ecaa62eba3338d047350d",
        "Tokoro-san no Setagaya C.C.",
        "(Japan) (GB Comp.)",
        "DMG-BGBJ",
        2436489991,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "91ff25c9ba4a9e309e269737b36acd3b5dd17225",
        "Godzilla - The Series - Monster Wars",
        "(USA) (En,Fr,De)",
        "CGB-BGDE",
        3890983828,
        1048576,
        "En,Fr,De",
        "USA"
   , " " },
    {
        "75e9e73fb52a67f8da464d298c8af983a708e35c",
        "Godzilla - The Series - Monster Wars",
        "(Europe) (En,Fr,De)",
        "CGB-BGDP",
        2785049825,
        1048576,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "10415eed7ed78cd341d1356e3c2946ca99836b8f",
        "Bubble Ghost",
        "(USA, Europe)",
        "DMG-BGE",
        2217765117,
        32768,
        "En",
        "USA, Europe"
   , " " },
    {
        "a60c0577eebb5979ef4594997d39a9509ca0f584",
        "Grandia - Parallel Trippers",
        "(Japan)",
        "CGB-BGEJ",
        589444720,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "f43d8f461f42b53831699164f668cd94b92836b9",
        "Gifty",
        "(Germany) (En)",
        "CGB-BGFD",
        3111816376,
        1048576,
        "En",
        "Germany"
   , " " },
    {
        "1b56ef28a0077c8aa825a709fe6ef99da66a9667",
        "Gift",
        "(Europe)",
        "CGB-BGFP",
        452998376,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "8ca135c4345ec836a21f77195f1d8ec34ff12d74",
        "Galaga - Destination Earth",
        "(USA)",
        "CGB-BGGE",
        3821317062,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "e5613ba5d53feac3944b907fef061274f996e08b",
        "Grinch, The",
        "(USA)",
        "CGB-BGHE",
        3315890326,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "069dcdaf955a5cd7f35c8a2fa15cd01a1acf2c09",
        "Grinch, The",
        "(Europe) (En,Fr,De)",
        "CGB-BGHP",
        2664826266,
        1048576,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "4f3e060477c6d592ab94b9ae173de9ff00e7a64f",
        "Gaiamaster Duel - Card Attackers",
        "(Japan)",
        "CGB-BGIJ",
        2015388768,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "5708b4d50e6bb7f52ca781f6a727345219434828",
        "Bubble Ghost",
        "(Japan)",
        "DMG-BGJ",
        2269973871,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "9fed9513ffc0e0c5827e667f24d8d03d37ecf6fa",
        "Doraemon no Study Boy - Gakushuu Kanji Game",
        "(Japan)",
        "CGB-BGKJ",
        1732329317,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "afbd71cf5358b6bf7c18f3daa043f6f27da06e45",
        "Super Gals! Kotobuki Ran",
        "(Japan)",
        "CGB-BGLJ",
        2419563542,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "6a0d63b119fb223e6e8c553e4ff5345b12759fe0",
        "Daiku no Gen-san - Kachikachi no Tonkachi ga Kachi",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-BGNJ",
        3792114323,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "2c5d26be646989dfc87ec0c3b690112d3c40ee7a",
        "Mobile Golf",
        "(Japan)",
        "CGB-BGOJ",
        905730866,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "399248a6d4d3044ec4a0025fc53654970d1e32bd",
        "Gremlins - Unleashed",
        "(Europe) (En,Fr,De,Es,It,Pt)",
        "CGB-BGPP",
        87802645,
        1048576,
        "En,Fr,De,Es,It,Pt",
        "Europe"
   , " " },
    {
        "fcf33397d92575c6ff3bfd3c3edc020dac0e737b",
        "Gute Zeiten Schlechte Zeiten Quiz",
        "(Germany) (GB Comp.)",
        "DMG-BGQD",
        1595122388,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "f63d47812739ba4e74633286fa98c9d9419e1218",
        "Gensou Maden Saiyuuki - Sabaku no Shijin",
        "(Japan)",
        "CGB-BGSJ",
        2441756903,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "9888dddea0619c4d0dfb7ae07240c30cd234835e",
        "Grand Theft Auto 2",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BGTP",
        2908109785,
        2097152,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "f0c0a65eee169cdadd58b27e52d43b9ce71859de",
        "Super Gals! Kotobuki Ran 2 - Miracle Getting",
        "(Japan)",
        "CGB-BGUJ",
        3883901170,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "8ac98181c9eb6422ceb607c47d7ed30584805dd9",
        "Gambler Densetsu Tetsuya - Shinjuku Tenun Hen",
        "(Japan) (Rev 1)",
        "CGB-BGYJ",
        2440273391,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "f09d035e35e0c5e59227e638f058d635e1d9a69b",
        "Gambler Densetsu Tetsuya - Shinjuku Tenun Hen",
        "(Japan)",
        "CGB-BGYJ",
        674589008,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "06fea297b516ee5697afcbc44d316ee2bb8c7247",
        "Zoids - Jashin Fukkatsu! Genobreaker Hen",
        "(Japan) (Rev 1) (SGB) (GB Comp.)",
        "DMG-BGZJ",
        2521176344,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "deddf74be2aaf54266a477ded01d460b708a3ae2",
        "Zoids - Jashin Fukkatsu! Genobreaker Hen",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-BGZJ",
        4083976013,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "85952aad4f07041b0cd5819ad19a033dda2ef1b0",
        "Frogger 2",
        "(USA) (Rev 1)",
        "CGB-BH2E",
        3810687869,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "b36267dea16a2ffc2d4e9a000929f631a969acc1",
        "Frogger 2",
        "(USA)",
        "CGB-BH2E",
        1780901546,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "2ed9f58269f8b6ae6d126abcd03062027eeb88ed",
        "Hamster Paradise 3",
        "(Japan)",
        "CGB-BH3J",
        348833404,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "87509fe184836b65e49bc8e78f14e6d1e33a4952",
        "Goukaku Boy Series - Shikakui Atama o Maruku Suru - Rika Battle Hen",
        "(Japan) (IE Institute)",
        "DMG-BH4J",
        1655329956,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "1ca9fa4cf9fd2abdcaee522838349fdd079aa4cb",
        "Harry Potter and the Chamber of Secrets",
        "(USA, Europe) (En,Fr,De,Es,It,Nl,Pt,Sv,Da)",
        "CGB-BH6E",
        326016268,
        4194304,
        "En,Fr,De,Es,It,Nl,Pt,Sv,Da",
        "USA, Europe"
   , " " },
    {
        "561227ad5c3cf17a4a3fa1d467f910f2818d500e",
        "Hero Hero-kun",
        "(Japan)",
        "CGB-BH9J",
        2360073380,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "bfc0ea6687af9e7349a7af3dc8d906ec91f3767a",
        "Super Me-Mail GB - Me-Mail Bear no Happy Mail Town",
        "(Japan)",
        "CGB-BHAJ",
        828156440,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "5a24c8ecbf899af458eb3514c03437b23d42b248",
        "Hercules - The Legendary Journeys",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BHCP",
        3236816671,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "f1808425a3cd937e0f2acfad1f6fb74f98d94e9c",
        "Harley-Davidson Motor Cycles - Race Across America",
        "(USA)",
        "CGB-BHDE",
        1682680964,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "25dbc1f4a9e6cb97d75d3ce3ca10c632482b0815",
        "Shin Megami Tensei Devil Children - Kuro no Sho",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-BHEJ",
        1438232401,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "3c57bc7960229d4133d5ee496d02a1ce33069e23",
        "Shin Megami Tensei Devil Children - Kuro no Sho",
        "(Japan) (Rev 1) (SGB) (GB Comp.)",
        "DMG-BHEJ",
        1028318518,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "3a986cff3be2a33a59bf5ddbcefe3a77d8707377",
        "Mary-Kate and Ashley - Winners Circle",
        "(USA, Europe)",
        "CGB-BHFE",
        903273932,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "620e47609d49116b889ec6d33cab20f1d68b2852",
        "Jankyuusei - Cosplay Paradise",
        "(Japan)",
        "CGB-BHGJ",
        3591248906,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "71a51d0a78be9a746c8de0aa3c62361d37bac704",
        "Hunter x Hunter - Hunter no Keifu",
        "(Japan)",
        "CGB-BHHJ",
        2704676418,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "8d5208fc1d32cd40c642d2d1f3d27008bd906a54",
        "NBA Hoopz",
        "(USA)",
        "CGB-BHIE",
        1343333401,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "850e45a3d8f7572f4154bd71f711f18f0898a5d5",
        "Ippatsu Gyakuten! DX Bakenou",
        "(Japan)",
        "DMG-BHJ",
        2403237781,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "c3b4a915e95d34312c6b68ab32dcb35c83290a6e",
        "Guruguru Town Hanamaru-kun",
        "(Japan)",
        "CGB-BHJJ",
        66845854,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "4b8e947fabc25d1fe5c810d08818785606aa25b2",
        "Hunter x Hunter - Kindan no Hihou",
        "(Japan)",
        "CGB-BHKJ",
        3527174773,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "6113df09b19dd2314dd48fa02b26382ebe21eef9",
        "Hamster Paradise 2",
        "(Japan)",
        "CGB-BHMJ",
        1412200630,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "778f71e844338f1382db7bb6d164a307649d6ba0",
        "Hamster Paradise 2",
        "(Japan) (Rev 1)",
        "CGB-BHMJ",
        3118592944,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "51f8f0b5eea55de7489ec9dafb4084944cbe945a",
        "Shin Megami Tensei Devil Children - Aka no Sho",
        "(Japan) (Rev 1) (SGB) (GB Comp.)",
        "DMG-BHNJ",
        3252843862,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "c84e1122bee8b8f25b6d043d8a6a70436bd170e5",
        "Shin Megami Tensei Devil Children - Aka no Sho",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-BHNJ",
        4178332023,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "e55fa0b7c69de68c3509a268608f313f7e9826e5",
        "Geheimnis der Happy Hippo-Insel, Das",
        "(Germany)",
        "CGB-BHOD",
        632586435,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "3599bea05516f806cafbe86b4a66ea660351f963",
        "Hissatsu Pachinko Boy - CR Monster House",
        "(Japan)",
        "CGB-BHPJ",
        1894843157,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "df8025623ffb93c14228cf79d748d4b6505fce8e",
        "Hype - The Time Quest",
        "(Europe) (En,Fr,De,Es,It,Nl,Sv,Da)",
        "CGB-BHQP",
        612658533,
        1048576,
        "En,Fr,De,Es,It,Nl,Sv,Da",
        "Europe"
   , " " },
    {
        "68670ef4154357abdc233c18de21299631068950",
        "GB Harobots",
        "(Japan)",
        "CGB-BHRJ",
        469985530,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "6ffb0047ee2943fbdd2129fcdb689fb0a9e73e26",
        "Senkai Ibunroku Juntei Taisen - TV Animation Senkaiden Houshin Engi Yori",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-BHSJ",
        603610963,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "4bf887f17046c928cc653ccf9e8e36c5c9257150",
        "Tottoko Hamutarou - Tomodachi Daisakusen Dechu",
        "(Japan) (Rev 1)",
        "CGB-BHTJ",
        3578388596,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "52f3f57bb40594354b510ada6530758ee9ff4abe",
        "Tottoko Hamutarou - Tomodachi Daisakusen Dechu",
        "(Japan)",
        "CGB-BHTJ",
        434849046,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "d4ce5005f8cec7c8291551c311661fe228ed42eb",
        "Hugo - Black Diamond Fever",
        "(Europe) (En,Fr,De,Es,It,Nl,Pt,Sv,No,Da,Fi)",
        "CGB-BHUP",
        1750904780,
        1048576,
        "En,Fr,De,Es,It,Nl,Pt,Sv,No,Da,Fi",
        "Europe"
   , " " },
    {
        "25e164d6a951aaf7802bebbc59d62b01a5226fed",
        "Harry Potter and the Sorcerer's Stone",
        "(USA, Europe) (En,Fr,De,Es,It,Nl,Pt,Sv,No,Da,Fi)",
        "CGB-BHVE",
        1339602885,
        4194304,
        "En,Fr,De,Es,It,Nl,Pt,Sv,No,Da,Fi",
        "USA, Europe"
   , " " },
    {
        "f9445f37b8cbd8ff51053083a0ee25ab71fd695f",
        "Harry Potter to Kenja no Ishi",
        "(Japan)",
        "CGB-BHVJ",
        765618282,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "d9ab970882e6afda432e66d92cf2e8d0d8e076f9",
        "Shin Megami Tensei Devil Children - Shiro no Sho",
        "(Japan)",
        "CGB-BHWJ",
        966854741,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "47acfd133ec2cc09a3c8a6ff37dfc5fc130b1010",
        "Hugo - The Evil Mirror",
        "(Europe) (En,Fr,De,Es,It,Nl,Pt,Sv,No,Da,Fi)",
        "CGB-BHXP",
        616000720,
        1048576,
        "En,Fr,De,Es,It,Nl,Pt,Sv,No,Da,Fi",
        "Europe"
   , " " },
    {
        "f2b4f7145231813eaab99b21d345ebfc708e5916",
        "Momotarou Densetsu 1-2",
        "(Japan)",
        "CGB-BI2J",
        3665801359,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "bf483a67bfa98d101fca72c900ca1e73276eb323",
        "McDonald's Monogatari - Honobono Tenchou Ikusei Game",
        "(Japan)",
        "CGB-BI9J",
        1714587285,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "9df0cf7becbf9ed17e229f8b587133cffa41f27f",
        "Bomber King - Scenario 2",
        "(Japan)",
        "DMG-BIA",
        3103690871,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "ef1d9cc0f79accc39d23f12f74ecd0657eee8b14",
        "Bibi Blocksberg - Im Bann der Hexenkugel",
        "(Germany)",
        "CGB-BIBD",
        1295645694,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "9c78bbeefa68d0ae3b9f6e48a7e158f2754da85a",
        "Top Gun - Fire Storm",
        "(USA, Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BICE",
        3803011599,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "USA, Europe"
   , " " },
    {
        "6c175461245e0500a6c46576b2d92d5d6c53bc8e",
        "Alone in the Dark - The New Nightmare",
        "(USA) (En,Fr,Es)",
        "CGB-BIDE",
        3242573878,
        4194304,
        "En,Fr,Es",
        "USA"
   , " " },
    {
        "b132559e1e70a76c21882aef150bdf5e5b3a08b7",
        "Alone in the Dark - The New Nightmare",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BIDP",
        2146567215,
        4194304,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "6a4fc6d09be07f516af0cc3b6f024a2490eaccb3",
        "Blaster Master Boy",
        "(USA)",
        "DMG-BIE",
        992768280,
        131072,
        "En",
        "USA"
   , " " },
    {
        "08744bcc5e5bd336aa17296e76fc81abf3622c72",
        "Inspector Gadget - Operation Madkactus",
        "(USA)",
        "CGB-BIGE",
        451982473,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "13fce65f7cae76d236776a9824343b6c5a181573",
        "Inspector Gadget - Operation Madkactus",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BIGP",
        865120342,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "2622ecf9223ad7c0100e16f3ff364ae37f6cd6b6",
        "Indiana Jones and the Infernal Machine",
        "(USA, Europe) (En,Fr,De)",
        "CGB-BIJE",
        2147422530,
        1048576,
        "En,Fr,De",
        "USA, Europe"
   , " " },
    {
        "4cc49caa78bbe7b4edb7e789eda632ecd3fe86f4",
        "International Karate 2000",
        "(Europe)",
        "CGB-BIKP",
        316672008,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "80132d049e9ef66d5f0a1dab2d800e47b7a8d8d4",
        "Ide Yousuke no Mahjong Kyoushitsu GB",
        "(Japan)",
        "CGB-BIMJ",
        1642039498,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "62d71ddf2dce477eeba7177f84685a6a38b5ca05",
        "Biohazard Gaiden",
        "(Japan)",
        "CGB-BIOJ",
        3260226870,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "686b7a1a1cb806dcff9e60b0731cdeb471d0b482",
        "Resident Evil Gaiden",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BIOP",
        4166795052,
        2097152,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "867afe194dd2423fcb140a64168d1c050858385d",
        "Rainbow Islands",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BISP",
        1844233907,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "4d7d20a02c6b945601cb5ddf001380316c26fd81",
        "Blaster Master Jr.",
        "(Europe)",
        "DMG-BIX",
        3925410159,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "d5610972043a12077944cb41f54e0a49d7bb8390",
        "David Beckham Soccer",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BJ2P",
        1966857444,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "84911d0648e94b93a08be4d01f64a307c3ce36b6",
        "Goukaku Boy GOLD - Shikakui Atama o Maruku Suru - Joushiki no Sho",
        "(Japan) (IE Institute)",
        "DMG-BJ3J",
        645344772,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "893a51200b9aad58365a1fb65ffc8c12f4265817",
        "NBA Jam 2001",
        "(USA, Europe)",
        "CGB-BJAE",
        984047388,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "643bbd88a16cf4cb0bed235e8b08aa380a9d7950",
        "Taito Memorial - Bubble Bobble",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-BJBJ",
        948725600,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "b3c32030d6e328300f3a754531f44f17de65f974",
        "Taito Memorial - Chase H.Q. - Secret Police",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-BJCJ",
        1779181357,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "53b3086383e95c9e4f5cbb6da2ae1e496262c169",
        "Bo Jackson - Two Games in One",
        "(USA)",
        "DMG-BJE",
        2128312491,
        131072,
        "En",
        "USA"
   , " " },
    {
        "9f79840b50cedf1a312b6631ee4784cf5470cc55",
        "Jet de Go! - Let's Go by Airliner",
        "(Japan)",
        "CGB-BJEJ",
        549768438,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "9e1301bcdb0c00ba9f98c38052850b39b45845e5",
        "Zen-Nihon Shounen Soccer Taikai - Mezase Nihon Ichi!",
        "(Japan)",
        "CGB-BJFJ",
        4021127988,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "f29d91966084e36e11f55023f1c9ecdd6dfe7eef",
        "Jungle Book, The - Mowgli's Wild Adventure",
        "(USA) (En,Fr,De,Es,It)",
        "CGB-BJGE",
        2607510879,
        4194304,
        "En,Fr,De,Es,It",
        "USA"
   , " " },
    {
        "9e3945ea0a1cb3fa55e30f4d4ae94e4954112e1c",
        "Jungle Book, The - Mowgli's Wild Adventure",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BJGP",
        3054103786,
        4194304,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "43ad421548a707f933911846f54fd1f228d6f0f8",
        "Hamster Club - Awasete Chuu",
        "(Japan)",
        "CGB-BJHJ",
        3580717311,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "cba872417ebe0a0debcdf637661cc05b6b2785ae",
        "Stuart Little - The Journey Home",
        "(USA, Europe)",
        "CGB-BJIE",
        3945216135,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "cb0daa6a3114d937e55df77497b73c8d1841abe8",
        "Stuart Little - The Journey Home",
        "(Europe) (Fr,De)",
        "CGB-BJIX",
        2897249894,
        1048576,
        "Fr,De",
        "Europe"
   , " " },
    {
        "01426f62e567b1f68c2cd925dfd9f0666ed1db23",
        "Powerpuff Girls, The - Bad Mojo Jojo",
        "(USA) (Rev 1)",
        "CGB-BJJE",
        1418340474,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "641ac93a4744bb9d85245ffc4449793e9dde0cee",
        "Powerpuff Girls, The - Bad Mojo Jojo",
        "(USA)",
        "CGB-BJJE",
        2202840954,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "989579655b00c0e19ee93ed7a87ad43c07f53dde",
        "Powerpuff Girls, The - Bad Mojo Jojo",
        "(USA) (Rev 2)",
        "CGB-BJJE",
        2702274639,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "f63ddc832e839373669a08181a41ac812c1b1861",
        "Powerpuff Girls, The - L'Affreux Mojo Jojo",
        "(France)",
        "CGB-BJJF",
        627906159,
        2097152,
        "Fr",
        "France"
   , " " },
    {
        "30864df0d21e9d0d31f2eef08033d2beb2a11aee",
        "Powerpuff Girls, The - Bad Mojo Jojo",
        "(Europe) (BJJP)",
        "CGB-BJJP",
        1976632890,
        2097152,
        "En",
        "Europe"
   , " " },
    {
        "73d890cec7ae5374c171bc7297da262b854aaba0",
        "Super Nenas, Las - El Malvado Mojo Jojo",
        "(Spain)",
        "CGB-BJJS",
        4024849087,
        2097152,
        "Es",
        "Spain"
   , " " },
    {
        "fd9bc41fb8c47ae13e4903be4d0274cd2c7b888c",
        "Jaguar Mishin Sashi Senyou Soft - Mario Family",
        "(Japan)",
        "CGB-BJMJ",
        2873048303,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "0507bd52662b605736576257946749e43175384f",
        "Hamster Club 2",
        "(Japan) (GB Comp.)",
        "DMG-BJNJ",
        800299257,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "a1bdec8fb52d1f37b02c4c294a7a757be3f2084f",
        "Pop'n Pop",
        "(Japan)",
        "CGB-BJPJ",
        486520676,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "84029e1755443c94f8dc157a6c6da87ca6fac512",
        "JumpStart Dino Adventure - Field Trip",
        "(USA)",
        "CGB-BJQE",
        1833989933,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "0ba21be4345e263b8fa366ad8f9c637fa8507e79",
        "Hamster Club - Oshiema Chuu",
        "(Japan)",
        "CGB-BJSJ",
        2043604814,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "17447f3ed5f87a4665e82bfa6a899284622f6ac6",
        "Jissen ni Yakudatsu Tsumego",
        "(Japan) (Rev 1)",
        "CGB-BJTJ",
        1441767518,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "570aecb892bffc41373f95da9d55f29dd2a0463d",
        "Jimmy White's Cueball",
        "(Europe)",
        "CGB-BJWP",
        660811599,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "1366dc58eec16ecb5bacb5bc4ee15096b026a4c9",
        "Jay und die Spielzeugdiebe",
        "(Germany)",
        "CGB-BJYD",
        1945433818,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "3c7e1d998a0f9cc4c012cee6731cdd3e1d60a82e",
        "Croc 2",
        "(USA, Europe)",
        "CGB-BK2E",
        3252027689,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "aa563364868fbab672a6bba9b881a00b281d3c26",
        "Kanji Boy 3",
        "(Japan)",
        "CGB-BK3J",
        760055191,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "45cdac5844f8aa7d4d3f4350a06b39e4d7d2c128",
        "Goukaku Boy GOLD - Shikakui Atama o Maruku Suru - Kanji no Tatsujin",
        "(Japan) (Imagineer)",
        "DMG-BK4J",
        1655705449,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "95a20b3dc1179d45469547bcaa6e62155d4277bb",
        "Hello Kitty no Happy House",
        "(Japan)",
        "CGB-BK7J",
        362800615,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "f96c30ccd279ffa6963c7551ac60a23982157f8f",
        "Hello Kitty to Dear Daniel no Dream Adventure",
        "(Japan)",
        "CGB-BK8J",
        26144041,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "c42fd1057d984f772293c9c569b6a1774641a542",
        "Kanji de Puzzle",
        "(Japan) (GB Comp.)",
        "DMG-BKAJ",
        636461527,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "9c115504ba9af9dd59775d063170f3cf840e32f9",
        "Rokumon Tengai Mon-Colle-Knight GB",
        "(Japan)",
        "CGB-BKDJ",
        3834306445,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "2af722fd2bd22f861ede26361384ec7a79fe286c",
        "Bases Loaded",
        "(USA)",
        "DMG-BKE",
        3548000223,
        131072,
        "En",
        "USA"
   , " " },
    {
        "baccd913db06fdcf0e84e85a536fa700dff76a92",
        "Hiryuu no Ken - Retsuden GB",
        "(Japan)",
        "CGB-BKEJ",
        3182334440,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "fc83a1ef852db69c6b4c8d7161251c94f1acf7de",
        "No Fear - Downhill Mountain Biking",
        "(Europe)",
        "CGB-BKFP",
        258602356,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "d9fc7ac798b5a1654715ae7f44227d0d3be15214",
        "Koguru Guruguru - Guruguru to Nakayoshi",
        "(Japan) (SGB Enhanced, GB Comp.) (NP)",
        "DMG-BKGJ",
        3590655844,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "47c09c6153b5842a323a9426771c652a06866df4",
        "Kisekae Series 3 - Kisekae Hamster",
        "(Japan)",
        "CGB-BKHJ",
        2728232495,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "eb3ac0497d0cf04f17c229cebc56651bcc52e784",
        "Kelly Club",
        "(USA)",
        "CGB-BKIE",
        1849289668,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "964cb16784733a171c8034cc0d8e5357366dfe10",
        "Shelly Club",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BKIP",
        1987881538,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "e67f6018d86268a8cef4f9e2390dede1a2eb5e04",
        "Baseball Kids",
        "(Japan)",
        "DMG-BKJ",
        2768489223,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "dd453845f46fac20cca8a0db87fcc00b913c7515",
        "Kirikou",
        "(Europe) (En,Fr,De,Es,It,Pt)",
        "CGB-BKKP",
        2017181741,
        1048576,
        "En,Fr,De,Es,It,Pt",
        "Europe"
   , " " },
    {
        "7ffeb207409907f88ca3896bd1a38a70b70f910a",
        "Keep the Balance",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BKNP",
        3093882950,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "d08009f5ec4211bf6fc068aa0eb04d6adca85dc1",
        "K.O. - The Pro Boxing",
        "(Japan)",
        "CGB-BKOJ",
        1777372297,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "145b3c8762ff2e2d202debb11d712459bbb71f7d",
        "Watashi no Kitchen",
        "(Japan) (Rev 1)",
        "CGB-BKRJ",
        1481009636,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "55230268b029ab88cb02328210ac555c647fa25a",
        "Watashi no Kitchen",
        "(Japan)",
        "CGB-BKRJ",
        3161881381,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "cefe2e281420a7492b99ed8e0c0a8048663c2734",
        "Kindaichi Shounen no Jikenbo - 10 Nenme no Shoutaijou",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-BKSJ",
        3729981404,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "8f81b731e218b07366209d3ea20118275bab66e7",
        "Hello Kitty no Sweet Adventure - Daniel-kun ni Aitai",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-BKTJ",
        1173624219,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "3aacaa02437186b698e052cffad6f4f638fb1a3c",
        "Puzzle de Shoubuyo! - Wootama-chan",
        "(Japan) (GB Comp.)",
        "DMG-BKUJ",
        1944924226,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "89ff75bfe1d6e40fb68ae1d3019ae27dd9b0b1e8",
        "Doraemon no Study Boy - Kanji Yomikaki Master",
        "(Japan)",
        "CGB-BKYJ",
        1880452519,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "453feeca9b62680af1b157f851b7d057f3743017",
        "Phantom Zona",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-BKZJ",
        1044640351,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "2482f93422c4d396b4fe587606df8c1663beddd6",
        "LEGO Island 2 - The Brickster's Revenge",
        "(USA) (En,Fr,Es)",
        "CGB-BL2E",
        2974787559,
        1048576,
        "En,Fr,Es",
        "USA"
   , " " },
    {
        "d1d987a5a5513a6e2d9e070bfa2ba538bd58be43",
        "LEGO Island 2 - The Brickster's Revenge",
        "(Europe) (En,Fr,De,Es,It,Nl,Pt,Sv,No,Da)",
        "CGB-BL2P",
        2451225377,
        1048576,
        "En,Fr,De,Es,It,Nl,Pt,Sv,No,Da",
        "Europe"
   , " " },
    {
        "5afc01733de22f1963e3e97730e6b357896d2ff1",
        "Sylvanian Families 3 - Hoshi Furu Yoru no Sunadokei",
        "(Japan)",
        "CGB-BL3J",
        2884840331,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "396abf27d2e5dd022c1193821e428e7da0e3b334",
        "Blodia",
        "(Japan)",
        "DMG-BLA",
        1375694419,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "713f6a421ffdfdbfc06e702155cce09ae3874cbd",
        "Marvin Strikes Back!",
        "(USA) (En,Fr,Es)",
        "CGB-BLAE",
        297040012,
        2097152,
        "En,Fr,Es",
        "USA"
   , " " },
    {
        "a8a0d448a7828a0debbd9bee3edbb960199d84ab",
        "Looney Tunes Collector - Martian Revenge!",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BLAP",
        580200034,
        2097152,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "b0406866126cf82159c17e95cb0a3df65167e8e3",
        "Land Before Time, The",
        "(USA)",
        "CGB-BLBE",
        3169438486,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "a3ba286a1ddc85d6461becc7f01cc1dd437e8290",
        "Land Before Time, The",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BLBP",
        4065583234,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "e7da650bdecacd55e4951b01a4a195338165bace",
        "Lufia - The Legend Returns",
        "(USA)",
        "CGB-BLCE",
        1538145284,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "5c91f06754e1a007d9728b548f82fb4b3e37f014",
        "Estpolis Denki - Yomigaeru Densetsu",
        "(Japan)",
        "CGB-BLCJ",
        1415672854,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "ef98fa75a67bcb7acefee51130b01eae064cabb5",
        "Laura",
        "(Europe) (En,Fr,De,Es,It,Nl,Sv,Da)",
        "CGB-BLDP",
        2777449090,
        1048576,
        "En,Fr,De,Es,It,Nl,Sv,Da",
        "Europe"
   , " " },
    {
        "942d07dd5e6d75b604d20481c39daf4a174c4863",
        "Blade",
        "(USA, Europe)",
        "CGB-BLEE",
        768970879,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "f35ac6ff8f81a5d27863466009d57c7ea0665394",
        "Lufia - The Legend Returns",
        "(Europe) (En,De)",
        "CGB-BLFP",
        1192086301,
        2097152,
        "En,De",
        "Europe"
   , " " },
    {
        "739fb2b759f033b3f363bc73b6ea467e7e75906c",
        "Love Hina Party",
        "(Japan)",
        "CGB-BLHJ",
        55387782,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "334bfc8ecf7d1971f6f06b568de1be9c3f3d0d4c",
        "NFL Blitz 2001",
        "(USA)",
        "CGB-BLIE",
        1640387502,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "0bcea20a6d8b025702607f15ecbfefde617cc3a2",
        "Koenig der Loewen, Der - Simbas grosses Abenteuer",
        "(Germany)",
        "CGB-BLKD",
        3927040924,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "d7849c5c8e5c7f202efa2443f023ecb83e9d004d",
        "Lion King, The - Simba's Mighty Adventure",
        "(USA, Europe)",
        "CGB-BLKE",
        3585390523,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "cff08b3d8900d6165770ebeaebb222ea61ae3531",
        "Roi Lion, Le - La Formidable Aventure de Simba",
        "(France)",
        "CGB-BLKF",
        1684536224,
        1048576,
        "Fr",
        "France"
   , " " },
    {
        "f0a7b1743607e6cc0eef88ad8856d665a7e94450",
        "Roi Lion, Le - La Formidable Aventure de Simba",
        "(France) (Rev 1)",
        "CGB-BLKF",
        3349864883,
        1048576,
        "Fr",
        "France"
   , " " },
    {
        "4516742f64d415f3d17d69303275cb7d59c746b8",
        "Lucky Luke - Desperado Train",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BLLP",
        3464433802,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "bd127034740d26187d6656ad67135df8b83108dc",
        "Little Nicky",
        "(USA)",
        "CGB-BLNE",
        657524992,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "3abd3f5b46a9d5c10642dffa41db3832bee0a130",
        "LEGO Alpha Team",
        "(USA)",
        "CGB-BLPE",
        1837024283,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "22c51ebcfb69eee78bafc23ad59a1d99d7964f24",
        "LEGO Alpha Team",
        "(Europe) (En,Fr,De,Es,It,Nl,Pt,Sv,No,Da)",
        "CGB-BLPP",
        1762394995,
        1048576,
        "En,Fr,De,Es,It,Nl,Pt,Sv,No,Da",
        "Europe"
   , " " },
    {
        "c1b4683cf3e527a3386acc04553890f4af5b13ac",
        "Looney Tunes Racing",
        "(USA) (En,Fr,De,Es,It,Nl)",
        "CGB-BLQE",
        1719217374,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "USA"
   , " " },
    {
        "6159635a5fbfb8904f90e54312ee326c50242a78",
        "Looney Tunes Racing",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BLQP",
        2130561674,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "8c1f3003295320a8d7bbde13fcd33b7d7cd4fc77",
        "LEGO Racers",
        "(USA) (En,Fr,Es)",
        "CGB-BLRE",
        4136000265,
        1048576,
        "En,Fr,Es",
        "USA"
   , " " },
    {
        "5bb154e89abd94c7d07e65cca9ea97b2928ee897",
        "LEGO Racers",
        "(Europe) (En,Fr,De,Es,It,Nl,Pt,Sv,No,Da)",
        "CGB-BLRP",
        168861459,
        1048576,
        "En,Fr,De,Es,It,Nl,Pt,Sv,No,Da",
        "Europe"
   , " " },
    {
        "8a37a733dc20bcef32db9822516a9786787581c4",
        "Oide Rascal",
        "(Japan)",
        "CGB-BLSJ",
        2651298292,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "6fdc77cae356db34fd5e47ea5adc791a8b9e8cc0",
        "From TV Animation One Piece - Yume no Luffy Kaizokudan Tanjou!",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-BLUJ",
        3331566135,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "9a18aa5f0286c1c56ca1b9817e3b5b18b525773b",
        "From TV Animation One Piece - Yume no Luffy Kaizokudan Tanjou!",
        "(Japan) (Rev 1) (SGB) (GB Comp.)",
        "DMG-BLUJ",
        2766006277,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "41ccc6c70ee88ecf4b49a98d76308ab4808e2a18",
        "Sylvanian Families 2 - Irozuku Mori no Fantasy",
        "(Japan)",
        "CGB-BLVJ",
        4163713311,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "b895fb3545d792438f8f3f500ccfadb97a55cafe",
        "LEGO Stunt Rally",
        "(USA)",
        "CGB-BLYE",
        3657975648,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "6d1703112f53349895efbc62b2a6faeba45d6d68",
        "LEGO Stunt Rally",
        "(Europe) (En,Fr,De,Es,It,Nl,Pt,Sv,No,Da)",
        "CGB-BLYP",
        97255931,
        1048576,
        "En,Fr,De,Es,It,Nl,Pt,Sv,No,Da",
        "Europe"
   , " " },
    {
        "398a3c618a7b43941dbc4311768da5c1d61dee23",
        "Harvest Moon 2 GBC",
        "(Germany) (SGB) (GB Comp.)",
        "DMG-BM2D",
        849366057,
        2097152,
        "De",
        "Germany"
   , " " },
    {
        "9524016685e504d1ef95f1864177b84c8238dd75",
        "Harvest Moon 2 GBC",
        "(USA) (SGB) (GB Comp.)",
        "DMG-BM2E",
        143680032,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "53d19631de4e7ba5a20f61e9666eca1dd81d31d9",
        "Harvest Moon 2 GBC",
        "(Europe) (SGB) (GB Comp.)",
        "DMG-BM2P",
        369928592,
        2097152,
        "En",
        "Europe"
   , " " },
    {
        "aef3c52b7e071d186378bc245d70c428df885e6b",
        "Micro Machines V3",
        "(USA, Europe)",
        "CGB-BM3E",
        3087418451,
        2097152,
        "En",
        "USA, Europe"
   , " " },
    {
        "d007ed6209cc104c9164bdb97ae5b4b67c645078",
        "Metal Walker",
        "(USA) (GB Comp.)",
        "DMG-BM4E",
        1004962705,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "730c7892d39bc6d0c1588ec05dcb6219d0319769",
        "Mat Hoffman's Pro BMX",
        "(USA, Europe)",
        "CGB-BM5E",
        1364989017,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "3de9b2431918a7346c1db0ccf93ac66542b20246",
        "Mega Man Xtreme",
        "(USA, Europe) (GB Comp.)",
        "DMG-BM6E",
        978162901,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "2313d5f1f9132c03433f0fdb709e03b88ac0f2db",
        "Macross 7 - Ginga no Heart o Furuwasero!!",
        "(Japan)",
        "CGB-BM7J",
        729463716,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "42c05e76ff88ac5230822cf362292160ff58f827",
        "Mario Tennis",
        "(USA)",
        "CGB-BM8E",
        2810299964,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "8b6daf2935228211d6a421327bcd9ae3c0277e1e",
        "Mario Tennis GB",
        "(Japan)",
        "CGB-BM8J",
        419891554,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "38a606c91908cdbb6b710c2c4af77702156b0aca",
        "Mario Tennis",
        "(Europe)",
        "CGB-BM8P",
        84989441,
        2097152,
        "En",
        "Europe"
   , " " },
    {
        "f41373b72c3606bf19c129c8af90eb3726835664",
        "Mary-Kate and Ashley - Pocket Planner",
        "(USA, Europe) (GB Comp.)",
        "DMG-BMAE",
        2225635513,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "d4e8f1e1a4dd4eba3a30c71fd0f1241006b75e0b",
        "Merlin",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BMBE",
        3307801838,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "f15339e078ec33c2b4cfba38976acaca72316669",
        "Barbie - Magic Genie Adventure",
        "(USA)",
        "CGB-BMCE",
        1475453442,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "513eaf560309675eedfddb441e7ef701b1c19381",
        "Mr. Driller",
        "(USA)",
        "CGB-BMDE",
        1227624127,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "06356eeac5eed63f4ce6d049285db0a3b0b54f5d",
        "Mr. Driller",
        "(Japan)",
        "CGB-BMDJ",
        2000103855,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "d35d49182c23e717d89019793d63eeedfe764f4c",
        "Mr. Driller",
        "(Europe)",
        "CGB-BMDP",
        3187257436,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "35d9386367d353addf894c6552976b430794678e",
        "Microsoft - The Best of Entertainment Pack",
        "(USA)",
        "CGB-BMEE",
        3537407226,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "f8a3d29e2ddb290e3d60d842ee040c472b2d4edd",
        "Microsoft - The 6 in 1 Puzzle Collection Entertainment Pack",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BMEP",
        3998005796,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "cd8fe5c73bf90277946fa2364bd35d7691a04edd",
        "Kakurenbo Battle - Monster Tactics",
        "(Japan)",
        "CGB-BMFJ",
        2633660099,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "789a7871021bf807116684932f2bc9f26ac95e99",
        "Metal Gear Solid",
        "(USA)",
        "CGB-BMGE",
        78693846,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "d220608c53031f47210d321028755f366f9a99cb",
        "Metal Gear - Ghost Babel",
        "(Japan)",
        "CGB-BMGJ",
        2016540749,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "31ae66e2e5606061b2a789c747c5d1daa0d6a439",
        "Mia Hamm Soccer Shootout",
        "(USA)",
        "CGB-BMHE",
        3615060416,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "caa17b8e48a910f7199d23eba1cd27f90d74245b",
        "European Super League",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BMHP",
        1510702194,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "b4bcbe2437f0b3563a8e0706a068355bb6d9f994",
        "M&M's Minis Madness",
        "(Germany)",
        "CGB-BMID",
        2791765325,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "e637237ba303434a0600efbf04d4fe9074b43378",
        "M&M's Minis Madness",
        "(USA)",
        "CGB-BMIE",
        2252986272,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "32f28bbbceb722bb295432b299852d5e944da1b6",
        "M&M's Minis Madness",
        "(Europe)",
        "CGB-BMIP",
        1617443780,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "b77af6a8b76d571d745c678e1f4940e6c9b17fc6",
        "Maya the Bee - Garden Adventures",
        "(Europe) (En,Fr,De,Es)",
        "CGB-BMJP",
        338390779,
        1048576,
        "En,Fr,De,Es",
        "Europe"
   , " " },
    {
        "a603a35b2f164f05d4870e580287bceadf701dcb",
        "Sylvanian Melodies - Mori no Nakama to Odori Masho!",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-BMKJ",
        1842916497,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "3c73e904b03f7cd01c53611f7cd829978da9f20b",
        "Warriors of Might and Magic",
        "(USA) (En,Fr,De)",
        "CGB-BMME",
        4020198378,
        1048576,
        "En,Fr,De",
        "USA"
   , " " },
    {
        "777aa29af45016f6fc68260fb99d2e03538405b8",
        "Kawaii Pet Shop Monogatari 2",
        "(Japan) (Rev 1) (GB Comp.)",
        "DMG-BMNJ",
        1875778256,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "fe39712f6f96febc12326867aefe4a69802a2d8d",
        "Kawaii Pet Shop Monogatari 2",
        "(Japan) (GB Comp.)",
        "DMG-BMNJ",
        932306081,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "ed70a2874606748698219abae87428d0c9799c2b",
        "Who Wants to Be a Millionaire - 2nd Edition",
        "(USA)",
        "CGB-BMOE",
        1748023215,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "212f0fd60172f630c98c350f562b3db249592a31",
        "DX Monopoly GB",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-BMPJ",
        1058932588,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "3697a3f4851b6d680d2c7c2ea547af5d11713787",
        "Microsoft Pinball Arcade",
        "(USA)",
        "CGB-BMQE",
        1347376581,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "7ac8fd92b3455177c9d5514f4dbecf1ac0cae4ed",
        "Microsoft Pinball Arcade",
        "(Europe)",
        "CGB-BMQP",
        1320672020,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "8c69904d24c692a5bb5f2ccbfd16f41211685fe8",
        "Metal Gear Solid",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BMSP",
        377055575,
        4194304,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "8ac3fa23e1bdd27d33ef0e2dcea14dd252b9b274",
        "MTV Sports - Skateboarding Featuring Andy MacDonald",
        "(USA, Europe)",
        "CGB-BMTE",
        1950704115,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "8c423e05f4142cd22244ac3e545f6615649eaff8",
        "Jim Henson's Muppets",
        "(USA)",
        "CGB-BMUE",
        1011313775,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "b8e546014115028390881d6f000cb149f7a56944",
        "Net de Get - Minigame @ 100",
        "(Japan)",
        "CGB-BMVJ",
        1848890633,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "e59cf88b123f94bb52f0aa7523ba328fcfb6d6d3",
        "Pro Mahjong Tsuwamono GB 2",
        "(Japan)",
        "CGB-BMWJ",
        4029683445,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "e77c04356f7e19518154e7e1a524f5d6329df39c",
        "Dave Mirra Freestyle BMX",
        "(USA, Europe)",
        "CGB-BMXE",
        771154480,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "33cd96a86b5397e5dfde84cf679b334b02e16ecd",
        "Mummy, The",
        "(USA)",
        "CGB-BMYE",
        3334119207,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "3c0d740a54cef6d1e4a489e2b1b0677d32f7204e",
        "Mummy, The",
        "(Europe) (En,Fr,De)",
        "CGB-BMYP",
        2231141258,
        1048576,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "85970a80ad3ba79ffedb1c6012e2cbaa5aa728ce",
        "Microsoft - The Best of Entertainment Pack",
        "(Europe)",
        "CGB-BMZP",
        251844360,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "eadd56b63a28a3e4101aa5ac01bedefa7c9e188e",
        "NASCAR 2000",
        "(USA, Europe)",
        "CGB-BN2E",
        1423510092,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "c275cc9054fcca4425718a3201c666624a8af52e",
        "Nintama Rantarou - Ninjutsu Gakuen ni Nyuugaku Shiyou no Dan",
        "(Japan)",
        "CGB-BNAJ",
        1177671871,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "9cb5b79463d8fc539f6d9e262f1a902ebbd9c3c3",
        "Space-Net - Cosmo Blue",
        "(Japan)",
        "CGB-BNBJ",
        4127642473,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "c5c7bc08807cf5b22349fed3d643ac6a1ac519d3",
        "Prince Naseem Boxing",
        "(Europe) (En,Fr,De)",
        "CGB-BNCP",
        2447308627,
        2097152,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "28a613fdbf21bab0e42e0de46a7eb68da93b95ae",
        "Wizardry III - Diamond no Kishi",
        "(Japan)",
        "CGB-BNDJ",
        3561829782,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "74e33dd8f983e8dc3fcd3b9f7262441405e24daf",
        "Madden NFL 2001",
        "(USA)",
        "CGB-BNFE",
        370056764,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "f7ac7002205913574402729a48fe916f3c9e7013",
        "NASCAR Heat",
        "(USA)",
        "CGB-BNHE",
        4202935164,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "03c6e2a72f563b37a83f7f073cf7220fd2e329ee",
        "Banishing Racer",
        "(Japan)",
        "DMG-BNJ",
        1024715882,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "caccc1153161b28d36fe17f9672d7044d2e6e4a4",
        "Return of the Ninja",
        "(USA)",
        "CGB-BNJE",
        2692589314,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "49e7cb98ba2932652d506652ed6c218f0e6e8fb0",
        "Return of the Ninja",
        "(Europe)",
        "CGB-BNJP",
        2382660762,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "79aaa38c7d84395aea746b27de12b1892e8976fe",
        "Nations, The - Land of Legends",
        "(Europe) (En,De)",
        "CGB-BNLP",
        2350473033,
        1048576,
        "En,De",
        "Europe"
   , " " },
    {
        "8e16208aaf046fe06e6ce0b1165976c9b417b4ae",
        "Animastar GB",
        "(Japan)",
        "CGB-BNMJ",
        2974359792,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "cbf5ac79461880cdb96b42302ab787268cf63e73",
        "Magi Nation",
        "(USA)",
        "CGB-BNNE",
        1346520331,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "98c224c9450dc893cada2d117d0b1604391b5965",
        "Simpsons, The - Night of the Living Treehouse of Horror",
        "(USA, Europe)",
        "CGB-BNOE",
        3954133128,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "28a24fef7be4364f6cad4e424e0a458123e0bb40",
        "Nisemon Puzzle da Mon! - Feromon Kyuushutsu Daisakusen!",
        "(Japan)",
        "CGB-BNPJ",
        796746483,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "de81112c6f493403a2275c4215f4a22d0ad7174d",
        "NASCAR Racers",
        "(USA)",
        "CGB-BNRE",
        4086979663,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "f9d50bafb7a1891e10a85b1344da409798e4c493",
        "NSYNC - Get to the Show",
        "(USA)",
        "CGB-BNSE",
        4151347083,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "f91d0a4156a220d3b77878facabf4657fe5c3221",
        "Nicktoons Racing",
        "(USA)",
        "CGB-BNTE",
        2750446036,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "70b666ca15aabb6bfec509567c63f6ccdec1b11e",
        "NYR - New York Race",
        "(Europe) (En,Fr,De,Es,It,Pt)",
        "CGB-BNXP",
        1660248280,
        1048576,
        "En,Fr,De,Es,It,Pt",
        "Europe"
   , " " },
    {
        "82bd0979a707fff6062dfa75a3bc019d8866814d",
        "Naminori Yarou!",
        "(Japan) (NP)",
        "CGB-BNYJ",
        2891859837,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "80194113aa81864ac50eea1c8c2a817898d7bd6e",
        "Star Ocean - Blue Sphere",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-BO2J",
        2357058522,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "500c44b2c826afa215b822b04fd6153b18c16d1d",
        "007 - The World Is Not Enough",
        "(USA, Europe)",
        "CGB-BO7E",
        3761825382,
        2097152,
        "En",
        "USA, Europe"
   , " " },
    {
        "8fc6714ecc14fe5ffba84fe7153543db07ddadd9",
        "Azarashi Sentai Inazuma - Dokidoki Daisakusen!",
        "(Japan)",
        "CGB-BOAJ",
        2566274004,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "617a53989152ab5ab0c8337d44bca87596a6bd6f",
        "Bob the Builder - Fix it Fun!",
        "(USA)",
        "CGB-BOBE",
        2482649021,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "c9d6b064b56eac1cad24026d9a1924182ca8bc38",
        "Bob the Builder - Fix it Fun!",
        "(Europe) (En,Fr,De,Es,Nl)",
        "CGB-BOBP",
        3929275965,
        1048576,
        "En,Fr,De,Es,Nl",
        "Europe"
   , " " },
    {
        "2d9fefc21e480554dd9cc98f875096aaf75b38b3",
        "Bob the Builder - Fix it Fun!",
        "(Europe) (En,Sv,No,Da,Fi)",
        "CGB-BOBX",
        4078561315,
        1048576,
        "En,Sv,No,Da,Fi",
        "Europe"
   , " " },
    {
        "3efb42fefbcedec375ab67b227d7c94a32f8b789",
        "Bob the Builder - Fix it Fun!",
        "(Europe) (En,Fr,It)",
        "CGB-BOBY",
        2834028683,
        1048576,
        "En,Fr,It",
        "Europe"
   , " " },
    {
        "edc99048c48393636359d7eba9bbf590afab0abb",
        "Bionic Commando",
        "(USA)",
        "DMG-BOE",
        1104918011,
        262144,
        "En",
        "USA"
   , " " },
    {
        "358ea89dbd15a5abbb871609975d522cf07b73e6",
        "Cool Bricks",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BOGP",
        83785616,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "1576b6ecf40f0a55ce5b0e29a3409440af45b646",
        "Bionic Commando",
        "(Japan)",
        "DMG-BOJ",
        1062034564,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "5607162d381d226bdfd7897ffbe98d7723e4df3d",
        "Ojarumaru - Mangan Jinja no Ennichi de Ojaru!",
        "(Japan) (GB Comp.)",
        "DMG-BOJJ",
        3630499785,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "a176458a68097e9f49b1c9a1678c89a745d71692",
        "Original Moorhuhn Jagd, Die",
        "(Germany)",
        "CGB-BOMD",
        1900986884,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "25077f3d28a762db903229554c878cf05a0032ea",
        "Karamuchou wa Oosawagi! - Okawari!",
        "(Japan) (SGB Enhanced, GB Comp.) (NP)",
        "DMG-BOOJ",
        1817611782,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "794bc7cc7949c6c8551993ae3bf5b24b2c99b64c",
        "Ojarumaru - Tsukiyo ga Ike no Takaramono",
        "(Japan) (GB Comp.)",
        "DMG-BORJ",
        3983827410,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "238603b74dbc14cbdf5cb536463235b8826c8bf1",
        "Ottifanten - Kommando Stoertebeker",
        "(Germany)",
        "CGB-BOTD",
        1051475228,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "06d9fcc837a6aef3473c507f4534d56f16322ec0",
        "Star Wars Episode I - Obi-Wan's Adventures",
        "(USA)",
        "CGB-BOWE",
        241792386,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "e8bc936609a81ffa830b36dc236dfba6e5023369",
        "Star Wars Episode I - Obi-Wan's Adventures",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BOWP",
        3850690498,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "3d31f0275406162b068bbd69493179744a9d2089",
        "Bionic Commando",
        "(Europe)",
        "DMG-BOX",
        1281049927,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "2d87364f1e7597ba556e857357df615217c4cf30",
        "Daikaijuu Monogatari - Poyon no Dungeon Room 2",
        "(Japan)",
        "CGB-BP2J",
        1580279600,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "226c4b1460d60332dea11aebd6f8d5ca598db9f9",
        "Puzzled",
        "(USA)",
        "CGB-BP3E",
        4030725551,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "615717f4cd01f4937eb4d37c907df7beabb4bdc2",
        "Puzzle Bobble Millennium",
        "(Japan)",
        "CGB-BP5J",
        16641428,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "45b610147ca226260a90e69a1d999187802fb224",
        "Doki x Doki Sasete!!",
        "(Japan)",
        "CGB-BP6J",
        430391136,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "57-56596",
        "Pokemon Card GB 2 - GR Dan Sanjou!",
        "(Japan)",
        "CGB-BP7J",
        1821587988,
        2097152,
        "Ja",
        "Japan",
        "/pokemon_tcg264.sprite" 
    },
    {
        "8c1ebff10ab74c560f9efc89bbbcd2bb0bd3a5df",
        "Pooh and Tigger's Hunny Safari",
        "(USA)",
        "CGB-BP8E",
        1646694618,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "8ec7bd6740485a0f7946877e085105b63ab7d363",
        "Pooh and Tigger's Hunny Safari",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BP8P",
        2231750969,
        2097152,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "75be3acbec43c6ffe8feb60f3cf2fb4f1cd887a8",
        "Pocket Music",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BP9P",
        469455646,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "dd13f27b77abb8f15ac2144e172616e0f3cd8676",
        "Battle Pingpong",
        "(Japan)",
        "DMG-BPA",
        2088270788,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "1c167e36792a679a9fb45fc05c0f6235d8092e62",
        "Pop'n Music GB - Animation Melody",
        "(Japan)",
        "CGB-BPAJ",
        2602838439,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "1d647babcd7186a4fe5f3cfe6472da739174b3e6",
        "Pumuckls Abenteuer im Geisterschloss",
        "(Germany)",
        "CGB-BPCD",
        2281499684,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "db701363490e14b6011f8553f42cf5b0e94ff819",
        "Pop'n Music GB - Disney Tunes",
        "(Japan)",
        "CGB-BPDJ",
        1220789999,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "7ddc2c0192bf24e6ee2cf697cf9c91e422b30e57",
        "Barbie - Pet Rescue",
        "(USA)",
        "CGB-BPEE",
        533295820,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "2bcaaadfd6ef2623689cde1fca4d638dce8fe5b4",
        "Barbie - Pet Rescue",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BPEP",
        1223047374,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "19d13eddd59c4dc159e44628b7ccfac54c58c0eb",
        "Hype - The Time Quest",
        "(Brazil) (En)",
        "CGB-BPFE",
        3405479989,
        1048576,
        "En",
        "Brazil"
   , " " },
    {
        "c6c19c4fd3a03b3d7d629e63090c306d3af79f8a",
        "Love Hina Pocket",
        "(Japan)",
        "CGB-BPHJ",
        478640829,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "e8bd7ace24c29f45739fcc98aa98e6515d278ba1",
        "Love Hina Pocket",
        "(Japan) (Rev 1)",
        "CGB-BPHJ",
        1440549798,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "ca7e532fb8f0ff750f10a1f7f8248825b9034137",
        "Pop'n Pop",
        "(Europe)",
        "CGB-BPIP",
        4234672100,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "5c55d65b75d6b382d85100e8ac8c73f9201d2d82",
        "Project S-11",
        "(USA)",
        "CGB-BPJE",
        550429416,
        524288,
        "En",
        "USA"
   , " " },
    {
        "1a91d3840021490db762704e5b02584b7d2b80c5",
        "Pocket Racing",
        "(Europe)",
        "CGB-BPKP",
        4227429234,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "df8f0ec4a33c0003ee9819279c4b3b3973c0160b",
        "Pro Pool",
        "(USA) (En,Fr,De)",
        "CGB-BPLE",
        638712381,
        1048576,
        "En,Fr,De",
        "USA"
   , " " },
    {
        "6c933dac7cd41a14c8ed354ed945ce320a7eaf69",
        "Pro Pool",
        "(Europe) (En,Fr,De)",
        "CGB-BPLP",
        564451940,
        1048576,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "b4fdb122bb73f20757c19abde2d38b1fb8ae4325",
        "Pop'n Music GB",
        "(Japan)",
        "CGB-BPMJ",
        132565653,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "157-25250",
        "Pokemon Puzzle Challenge",
        "(USA, Australia)",
        "CGB-BPNE",
        3496721046,
        2097152,
        "En",
        "USA, Australia",
        "/pokemon_puzzle64.sprite" 
    },
    {
        "41-48878",
        "Pokemon de Panepon",
        "(Japan)",
        "CGB-BPNJ",
        1811408038,
        2097152,
        "Ja",
        "Japan",
        "/pokemon_puzzle64.sprite" 
    },
    {
        "66-14093",
        "Pokemon Puzzle Challenge",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BPNP",
        2181476814,
        2097152,
        "En,Fr,De,Es,It",
        "Europe",
        "/pokemon_puzzle64.sprite" 
    },
    {
        "a3cd5d280c12c1ff4f95308db069af559e5594a6",
        "Winnie the Pooh - Adventures in the 100 Acre Wood",
        "(USA)",
        "CGB-BPOE",
        107618710,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "77d05311bdaf05abb78c5febf13a02ef1c2d72f0",
        "Winnie the Pooh - Adventures in the 100 Acre Wood",
        "(Europe) (En,Fr,De,Es,It,Nl,Da)",
        "CGB-BPOP",
        498107404,
        2097152,
        "En,Fr,De,Es,It,Nl,Da",
        "Europe"
   , " " },
    {
        "b2eee035ff288718b7c4a6fba6378882d5597eca",
        "Pocket Pro Yakyuu",
        "(Japan)",
        "CGB-BPPJ",
        711915176,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "cdfe193793b9c938d86897a91d68f1c5c752fbf4",
        "Perfect Choro Q",
        "(Japan)",
        "CGB-BPQJ",
        2946607811,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "c8f0d876a5be91a85998f20369f0a2fe92e35e4d",
        "Pocket Soccer",
        "(Europe) (En,Fr,De,Es,It,Pt)",
        "CGB-BPSE",
        3908403791,
        2097152,
        "En,Fr,De,Es,It,Pt",
        "Europe"
   , " " },
    {
        "862002c915b6bdcdbc68037a6fe28c022a18e029",
        "Powerpuff Girls, The - Paint the Townsville Green",
        "(USA) (Rev 1)",
        "CGB-BPTE",
        693843526,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "8b8cafc3665d3997edeb2c2387f58db27c4309f3",
        "Powerpuff Girls, The - Paint the Townsville Green",
        "(USA) (Rev 2)",
        "CGB-BPTE",
        1144219566,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "ed454a2d0890180f912edee63766df86cc5dd459",
        "Powerpuff Girls, The - Paint the Townsville Green",
        "(USA)",
        "CGB-BPTE",
        2638685722,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "7dcf8bc59208f38f784d6f4499113c36c64e1126",
        "Powerpuff Girls, The - Panique a Townsville",
        "(France)",
        "CGB-BPTF",
        2040797147,
        2097152,
        "Fr",
        "France"
   , " " },
    {
        "25608013821283910d2ed4c39e1793304a38e835",
        "Powerpuff Girls, The - Paint the Townsville Green",
        "(Europe) (BPTP)",
        "CGB-BPTP",
        2565124292,
        2097152,
        "En",
        "Europe"
   , " " },
    {
        "7aad770174e5312a83c48d24888696f2391e06d6",
        "Super Nenas, Las - Panico en Townsville",
        "(Spain)",
        "CGB-BPTS",
        65910132,
        2097152,
        "Es",
        "Spain"
   , " " },
    {
        "8d3721a2c77c154470d10c1d2a25b2e525e8114a",
        "Atsumete Asobu Kuma no Pooh-san - Mori no Takaramono",
        "(Japan)",
        "CGB-BPUJ",
        1198448275,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "aae3c23dea68f19aa310dd79d0444df8719182ea",
        "MTV Sports - Pure Ride",
        "(USA, Europe)",
        "CGB-BPVE",
        3081909579,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "16f78656c0e641c7b7db474f234933697ff94d80",
        "Power Pro Kun Pocket 2",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-BPWJ",
        3265569771,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "73f2a0fd10b62e6fbf9153a2942901fe17547bfe",
        "Laura",
        "(USA)",
        "CGB-BPXE",
        3804230278,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "2d510b19b0bad02e13ccd8050901d71e0bda53c6",
        "Pocket Puyo Puyo-n",
        "(Japan) (Rev 1)",
        "CGB-BPYJ",
        3364094842,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "536c2c9cf2d35ab12b54f75948f4a42337f22be4",
        "Pocket Puyo Puyo-n",
        "(Japan) (Rev 2)",
        "CGB-BPYJ",
        1463161815,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "7f54ae1886efcc0ce3751761b97306ac6e8ed11e",
        "Pocket Puyo Puyo-n",
        "(Japan)",
        "CGB-BPYJ",
        2265822007,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "9726fc0f6e7d4a7fe8e9449e969fbe82cc22ea5a",
        "Puzz Loop",
        "(Japan) (GB Comp.)",
        "DMG-BPZJ",
        2944762420,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "a79d97c2a9deb586d195b8f1d32a35c05261ba52",
        "Goukaku Boy GOLD - Shikakui Atama o Maruku Suru - Kanji no Tatsujin",
        "(Japan) (IE Institute)",
        "DMG-BQ2J",
        403526823,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "e3cdb416689f1b544c3b23b21361b0cefc627214",
        "Doraemon no Quiz Boy 2",
        "(Japan)",
        "CGB-BQ7J",
        2659195290,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "6243943b4e4862462d451c746c89319c6604bc6e",
        "Donald Duck - Goin' Quackers",
        "(USA) (En,Fr,De,Es,It)",
        "CGB-BQAE",
        1183158427,
        4194304,
        "En,Fr,De,Es,It",
        "USA"
   , " " },
    {
        "d958ea5b32a14840fff7a55939f228c45211a4fb",
        "Donald Duck - Quack Attack",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BQAP",
        128007003,
        4194304,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "cbd51a9882e68bddc095645690f6eec30c91f67f",
        "Doraemon no Quiz Boy",
        "(Japan) (Rev 1)",
        "CGB-BQBJ",
        2438158051,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "d9470f442b37b68a85eea9965767300f93c18e00",
        "Doraemon no Quiz Boy",
        "(Japan)",
        "CGB-BQBJ",
        1002413736,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "b0c1df461f3d09a2df8292ed176c28bd3f627090",
        "Blues Brothers, The",
        "(USA, Europe)",
        "DMG-BQE",
        2914414335,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "40d3dc6b9ee1cf44234e88a7f53d60ee529c2d3d",
        "Dragon Warrior Monsters 2 - Tara's Adventure",
        "(USA) (SGB) (GB Comp.)",
        "DMG-BQIE",
        904683442,
        4194304,
        "En",
        "USA"
   , " " },
    {
        "93426e57634ee32fab666588d6feb477e60c78ff",
        "Dragon Quest Monsters 2 - Maruta no Fushigi na Kagi - Iru no Bouken",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-BQIJ",
        1757026519,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "dc821bd1b8c73c969ddb6d6659b96c9533661d32",
        "Dragon Warrior Monsters 2 - Cobi's Journey",
        "(USA) (SGB) (GB Comp.)",
        "DMG-BQLE",
        2877029845,
        4194304,
        "En",
        "USA"
   , " " },
    {
        "112d1edb7b402b7c230aa074bb7b589d707398aa",
        "Dragon Quest Monsters 2 - Maruta no Fushigi na Kagi - Ruka no Tabidachi",
        "(Japan) (Rev 1) (SGB) (GB Comp.)",
        "DMG-BQLJ",
        1688112535,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "b15e0ea6a86c5b70e4d0e5bf7f5c9cc019d97062",
        "Dragon Quest Monsters 2 - Maruta no Fushigi na Kagi - Ruka no Tabidachi",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-BQLJ",
        742558343,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "b87d0057bb7ed26e4fa57686ccf9a50557d7114d",
        "SpongeBob SquarePants - Legend of the Lost Spatula",
        "(USA, Europe)",
        "CGB-BQPE",
        2166556004,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "6879b7f8be9b96e2cdaddfe16e9c26a6a3b82349",
        "Doraemon no Study Boy - Kuku Game",
        "(Japan)",
        "CGB-BQQJ",
        2829276120,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "3347ade59c29500a26a30653edbe381504221a82",
        "Shinseiki Evangelion - Mahjong Hokan Keikaku",
        "(Japan)",
        "CGB-BQRJ",
        1396178773,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "8c894317a7c5c8302be08ac81e3e87b52d9f78ff",
        "Dejiko no Mahjong Party",
        "(Japan)",
        "CGB-BQSJ",
        3310362574,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "cabc30c7473383fba6bd15d342df19bc49aa7c69",
        "Q-bert",
        "(USA)",
        "CGB-BQTE",
        2431937918,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "12705962fd1ce1a706b16df7f8601db1fc710f2e",
        "Shaman King Card Game - Chou Senjiryakketsu - Funbari Hen",
        "(Japan)",
        "CGB-BQUJ",
        4010813230,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "2c78e7aab1b88246f05bcf9e40893c42133ce070",
        "Shaman King Card Game - Chou Senjiryakketsu - Meramera Hen",
        "(Japan)",
        "CGB-BQVJ",
        1462778173,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "9f47d2c8c8fe2318536ae687c1555b1d29c55351",
        "Sakura Taisen GB 2 - Thunderbolt Sakusen",
        "(Japan)",
        "CGB-BQYJ",
        1197697580,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "e20fa118c023b09d3c00a3fdc9025c793607b5e5",
        "RoboCop",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BR9P",
        1818502114,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "169d9f6a6cc986067ed53ecc829031811e906dc9",
        "Battle Bull",
        "(Japan)",
        "DMG-BRA",
        1550443593,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "937a5cd9aecab26a5f624c0f0af680614dd11f49",
        "Sewing Machine Operation Software",
        "(USA) (En,Fr,Es) (GB Comp.)",
        "DMG-BRAE",
        3828340676,
        1048576,
        "En,Fr,Es",
        "USA"
   , " " },
    {
        "8973ac5fa08675ba44fb59095a43f803fe4a8065",
        "Raku x Raku - Mishin",
        "(Japan) (GB Comp.)",
        "DMG-BRAJ",
        195064091,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "cf1eef467f25238c8a85da3015baa548d231b5e5",
        "Sewing Machine Operation Software",
        "(Europe) (En,Fr,De,It) (GB Comp.)",
        "DMG-BRAZ",
        3628285422,
        1048576,
        "En,Fr,De,It",
        "Europe"
   , " " },
    {
        "212fdd278f69ff25f0e7a0c627b388b009304077",
        "Arle no Bouken - Mahou no Jewel",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-BRBJ",
        2397062122,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "48c864b4efd3c9b5ad67bbc3d763b658fe74c0c3",
        "Croc",
        "(USA, Europe)",
        "CGB-BRCE",
        1181000039,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "22f97b4656579c9e76951aa01862a811a7e8c6c7",
        "Doraemon - Kimi to Pet no Monogatari",
        "(Japan) (GB Comp.)",
        "DMG-BRDJ",
        799302161,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "06d5565bc99002a58a23a92afe39b5e39d2976af",
        "Battle Bull",
        "(USA)",
        "DMG-BRE",
        3570511522,
        131072,
        "En",
        "USA"
   , " " },
    {
        "f2170ac11fc374fc748e5571bb08ae2dea7cbb59",
        "Space-Net - Cosmo Red",
        "(Japan)",
        "CGB-BREJ",
        33121107,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "a1334919b25fa70a7c0c9cad5727028701144122",
        "Sgt. Rock - On the Frontline",
        "(USA)",
        "CGB-BRFE",
        1377447799,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "6f4b97afbe6d7e2cf5c0acf7adebce72fb312427",
        "Roland Garros French Open",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BRGP",
        282844796,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "d0af8b94e8690645a2120c650d065dd4bc1d53fb",
        "Robin Hood",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BRHP",
        3573039913,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "e7914ccfb87cd5377d37d3f1c38d267956271042",
        "Sakura Taisen GB - Geki Hana Gumi Nyuutai!",
        "(Japan)",
        "CGB-BRJJ",
        4015013200,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "c7f438f9cf99371ec0252215e0da30e0b107211b",
        "Legend of the River King 2",
        "(USA) (SGB) (GB Comp.)",
        "DMG-BRKE",
        2215630117,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "8b1023a8d34ea17ffd1bedacb8b6ec663f8ca75f",
        "Legend of the River King 2",
        "(Europe) (SGB) (GB Comp.)",
        "DMG-BRKP",
        4065132810,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "253cdd27f3322cef9a0aa521fd4a625665e56d68",
        "Rocky Mountain - Trophy Hunter",
        "(USA)",
        "CGB-BRME",
        2594549793,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "fa09c411b68d626b83e082ac56dacf4e9c39d1ef",
        "Rhino Rumble",
        "(USA, Europe)",
        "CGB-BRNE",
        1930825221,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "a2871af628d9c256a929172bd099acbf5d5d032a",
        "Dexter's Laboratory - Robot Rampage",
        "(USA, Europe)",
        "CGB-BROE",
        3528287745,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "163fe6f72be0c99a96d3ba57071ed4e713938610",
        "Rugrats in Paris - The Movie",
        "(USA, Europe)",
        "CGB-BRPE",
        2333692471,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "119e8e4b890c47641fc6bc65b006b58f749a24cd",
        "Razmoket a Paris, Les - Le Film",
        "(France)",
        "CGB-BRPF",
        155489513,
        1048576,
        "Fr",
        "France"
   , " " },
    {
        "e05f7401200178774ef565a092491e5f74b2c213",
        "Rugrats en Paris - La pelicula",
        "(Spain) (En,Es)",
        "CGB-BRPX",
        892482610,
        1048576,
        "En,Es",
        "Spain"
   , " " },
    {
        "acacf6c6beb5e716857bb2bef97c88f879fa0fba",
        "Rescue Heroes - Fire Frenzy",
        "(USA)",
        "CGB-BRQE",
        2007293027,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "41e069799d3929b503e6970c3fa4b0db1ba11189",
        "Road Rash",
        "(USA, Europe)",
        "CGB-BRRE",
        285368043,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "e4d5d7ee74cd453dd005382f6bb378815457122d",
        "Saban's Power Rangers - Lightspeed Rescue",
        "(USA, Europe)",
        "CGB-BRSE",
        2575733106,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "5402ff604abcbc035e37aa0cb6fedbda4328cd49",
        "Razmoket, Les - Voyage dans le Temps",
        "(France)",
        "CGB-BRTF",
        797962047,
        1048576,
        "Fr",
        "France"
   , " " },
    {
        "8dfb7392b0bd7d2c97e5cf4b0a303f1c97c97117",
        "Rocket Power - Gettin' Air",
        "(USA, Europe)",
        "CGB-BRUE",
        1881533283,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "b8b422835de7c4c8dc2d5f1937c6b84a53c2024c",
        "Rocket Power - La Glisse de l'Extreme",
        "(France)",
        "CGB-BRUF",
        3383208571,
        1048576,
        "Fr",
        "France"
   , " " },
    {
        "f3260e7dad2268d28a70ac341e8872e4086c3ac0",
        "Robot Wars - Metal Mayhem",
        "(Europe) (En,Fr,De,It,Nl,Sv)",
        "CGB-BRWP",
        3919306470,
        1048576,
        "En,Fr,De,It,Nl,Sv",
        "Europe"
   , " " },
    {
        "a06f7ada8b348f721a3d536cd208b46885f176d8",
        "Rockman X - Cyber Mission",
        "(Japan) (GB Comp.)",
        "DMG-BRXJ",
        2442164139,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "991b8482c7a6a95072c40496768efc978a5714f8",
        "Rayman 2",
        "(USA) (En,Fr,De,Es,It)",
        "CGB-BRYE",
        1868312925,
        4194304,
        "En,Fr,De,Es,It",
        "USA"
   , " " },
    {
        "eb17bcdf5696ab2fdaa97b440ace04f52583762b",
        "Rayman 2 Forever",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BRYP",
        620762580,
        4194304,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "83634f4638bc2801db23bd00dcb431a584881e1f",
        "Razor Freestyle Scooter",
        "(USA)",
        "CGB-BRZE",
        1545605766,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "00f58cf62aefed5e6b6e52c57bea2fcbd8ec6b3e",
        "Freestyle Scooter",
        "(Europe)",
        "CGB-BRZP",
        4000911741,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "8097052bbd890d208b3bc9d62656b574fb1883f8",
        "Soreike! Anpanman - 5-tsu no Tou no Ousama",
        "(Japan)",
        "CGB-BS2J",
        3382860556,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "03a3d63655fe589aaff66f84e0120a913a0fd62f",
        "Goukaku Boy GOLD - Shikakui Atama o Maruku Suru - Joushiki no Sho",
        "(Japan) (Imagineer)",
        "DMG-BS4J",
        1564713861,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "7e5ee5b586cb17cd91c049096da21391690cf919",
        "Samurai Kid",
        "(Japan)",
        "CGB-BS5J",
        1151983099,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "f403f54a95531de29c0c08aa44793700de87c9c9",
        "Cardcaptor Sakura - Tomoeda Shougakkou Daiundoukai",
        "(Japan)",
        "CGB-BS7J",
        4153375128,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "4a02ade7339fb2a09de5ef2a66e8a07fca1e8414",
        "Snoopy Tennis",
        "(USA) (En,Fr,Es)",
        "CGB-BS9E",
        3632458956,
        1048576,
        "En,Fr,Es",
        "USA"
   , " " },
    {
        "0438c36fe5d5fc3d9172a42bf9f8d4f7131d9376",
        "Snoopy Tennis",
        "(Japan)",
        "CGB-BS9J",
        3498635005,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "15aff5608f8140ae67a19106c2245e8aa677b615",
        "Snoopy Tennis",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BS9P",
        2351799107,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "c5b536ee304c12c0857681feedc86825d0420fbe",
        "Baseball",
        "(World)",
        "DMG-BSA",
        1860838694,
        65536,
        "En",
        "World"
   , " " },
    {
        "a32cda583555679f3485562512a6718c755b79b4",
        "Dear Daniel no Sweet Adventure - Kitty-chan o Sagashite",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-BSAJ",
        265503783,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "2aa798a85eb6ebefba6f64fd32eab8e505344930",
        "Cyborg Kuro-chan 2 - White Woods no Gyakushuu",
        "(Japan)",
        "CGB-BSBJ",
        3899388206,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "259272fd5e631a43aeb39ff11730319d13bac818",
        "Spirou - The Robot Invasion",
        "(Europe) (En,Fr,De,Es,It,Nl,Da)",
        "CGB-BSCP",
        1050396939,
        1048576,
        "En,Fr,De,Es,It,Nl,Da",
        "Europe"
   , " " },
    {
        "ab8038b119bebe3e309b7b126aeb63eb047225ac",
        "Spider-Man",
        "(USA, Europe)",
        "CGB-BSEE",
        887272378,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "aaa54c2b10535f05054f80a5b754bff22f04c756",
        "Spider-Man",
        "(France)",
        "CGB-BSEF",
        4130557381,
        1048576,
        "Fr",
        "France"
   , " " },
    {
        "7670da225768b21ae08aa39b7db5f89084617201",
        "Spider-Man",
        "(Japan)",
        "CGB-BSEJ",
        1518591940,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "245-13069",
        "Adventures of the Smurfs, The",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BSFP",
        3503546349,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "9650646bb26a5e2ae62e41585c5264bb1f7d846d",
        "Sabrina - The Animated Series - Zapped!",
        "(USA, Europe)",
        "CGB-BSGE",
        1564060080,
        2097152,
        "En",
        "USA, Europe"
   , " " },
    {
        "20bab3e47d9c11d871625e7d2f70cbe38861c2b7",
        "Sabrina - The Animated Series - Zapped!",
        "(Europe) (En,Fr,De)",
        "CGB-BSGX",
        2173647606,
        2097152,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "75e65823ec9b24d18f5bc0897f6d9108530f9e8f",
        "Army Men - Sarge's Heroes 2",
        "(USA, Europe) (En,Fr,De)",
        "CGB-BSHE",
        1907906538,
        1048576,
        "En,Fr,De",
        "USA, Europe"
   , " " },
    {
        "25b0b5d61b3d5c868939e7f5891d71eaa1b7bcf7",
        "Space Invaders X",
        "(Japan) (En) (GB Comp.)",
        "DMG-BSIJ",
        3222715513,
        1048576,
        "En",
        "Japan"
   , " " },
    {
        "cfae333fe194876c38d0e27a2ff90ac26fe91173",
        "Bundesliga Stars 2001",
        "(Germany)",
        "CGB-BSJD",
        1445115217,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "e612688d10759e9a4d7f7e00d31d03a117f33dad",
        "LNF Stars 2001",
        "(France)",
        "CGB-BSJF",
        4173283047,
        1048576,
        "Fr",
        "France"
   , " " },
    {
        "537920ee6acf4e59575839a7a0b7e8d6d48094aa",
        "F.A. Premier League Stars 2001, The",
        "(Europe)",
        "CGB-BSJP",
        1580745979,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "9980fc080fc8c5f5afc5123d7b8ffd797794273b",
        "Cyborg Kuro-chan - Devil Fukkatsu",
        "(Japan)",
        "CGB-BSKJ",
        959275791,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "f0f35da09ceabb5220237effff4ac348cd134e5e",
        "Super Doll Licca-chan - Kisekae Daisakusen",
        "(Japan)",
        "CGB-BSLJ",
        3746580287,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "3059abd1efe5c10c6ffc1b70ce3c79a3f1da6b26",
        "Soccer Manager",
        "(Europe) (En,Fr,De,Es)",
        "CGB-BSMP",
        595513081,
        1048576,
        "En,Fr,De,Es",
        "Europe"
   , " " },
    {
        "f8f589c59168bc27326965e555d9fe6e72789ab9",
        "Mickey's Speedway USA",
        "(USA, Europe) (En,Fr,De,Es)",
        "CGB-BSNE",
        292960564,
        4194304,
        "En,Fr,De,Es",
        "USA, Europe"
   , " " },
    {
        "4445b777316634c4f293462d5bcdf5b543772568",
        "Monster Rancher Explorer",
        "(USA)",
        "CGB-BSOE",
        1815472368,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "f7a84e22dc5a46bfe9aa3ca85e9e5507ba71c192",
        "Solomon",
        "(Japan)",
        "CGB-BSOJ",
        1793984856,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "0f4a9994516555afb0e82ee9811278254509b2c7",
        "Saru Puncher",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-BSPJ",
        2955240700,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "4483ab859349b024da25db021092ac86c7430764",
        "Monkey Puncher",
        "(Europe) (SGB) (GB Comp.)",
        "DMG-BSPP",
        3155691982,
        2097152,
        "En",
        "Europe"
   , " " },
    {
        "7d6712d162a5005fe63bea6af1350840568146c1",
        "Sesame Street Sports",
        "(USA)",
        "CGB-BSQE",
        1322560250,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "4991c5af330d0a96f457a7c3cfbb232809d90937",
        "Sesame Street Sports",
        "(Europe)",
        "CGB-BSQP",
        1438521966,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "3c5bbf126971ceddd1dd9bf103b2e175236bd969",
        "Ultimate Surfing",
        "(USA)",
        "CGB-BSUE",
        3897422320,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "560e8693af127b472f8d5d10f21695443f121296",
        "Super Robot Pinball",
        "(Japan)",
        "CGB-BSUJ",
        1848840141,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "ce23070e20354cfca606fba79bb67ab5453392ea",
        "Ultimate Surfing",
        "(Europe)",
        "CGB-BSUP",
        3006892699,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "9c604152dba97084bff58fe23a88fd1359bf7161",
        "SWiV",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BSVP",
        1154681722,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "98246170e2ccd9bb7f77efc7c17ecf0f7b5053d0",
        "Swing",
        "(Germany)",
        "CGB-BSWD",
        1883345565,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "59bd4434c80b3d70378526d8293075d9bdb615b5",
        "Supercross Freestyle",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BSXP",
        1295912785,
        2097152,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "0e79fe91cf9b80f11bfc968f2ebc46a5e3b51f56",
        "Toonsylvania",
        "(USA)",
        "CGB-BSYE",
        158141479,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "c9cea623811701a82a8ae4476333612a4dc3e56a",
        "Toonsylvania",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BSYP",
        1702082703,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "e18924661404a69e6ca2014c1757ca465e21ecf2",
        "Taxi 2",
        "(France)",
        "CGB-BT2F",
        265945072,
        1048576,
        "Fr",
        "France"
   , " " },
    {
        "4a4897d5ba2d0c30c93f89febb643f87228397e3",
        "Turok 3 - Shadow of Oblivion",
        "(USA, Europe) (En,Fr,De,Es)",
        "CGB-BT3E",
        1833465438,
        1048576,
        "En,Fr,De,Es",
        "USA, Europe"
   , " " },
    {
        "9a8e951300ae3a0465898ba386150689f13f415a",
        "ESPN National Hockey Night",
        "(USA)",
        "CGB-BT4E",
        1611620857,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "1d094da47b5fbc978dea8efd1aa230413b3e3618",
        "Toy Story Racer",
        "(USA, Europe)",
        "CGB-BT5E",
        3641826711,
        2097152,
        "En",
        "USA, Europe"
   , " " },
    {
        "b623378696e7bffc60c20591839d6fe1d66d0fc6",
        "Toy Story Racer",
        "(Europe) (En,Fr,De)",
        "CGB-BT5X",
        4133547412,
        2097152,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "663fb65e5f2efbc957021685211d31d7edcce686",
        "Tintin au Tibet",
        "(Europe) (En,Fr,De,Es,It,Nl,Sv)",
        "CGB-BT6P",
        1748169610,
        1048576,
        "En,Fr,De,Es,It,Nl,Sv",
        "Europe"
   , " " },
    {
        "847655fef17091781b525729af51fe8b88510bd2",
        "Shin Megami Tensei Trading Card - Card Summoner",
        "(Japan)",
        "CGB-BT8J",
        2233878647,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "5528cec672aa4ed525ae5e23dd97226c82c60ef2",
        "Tootuff",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BT9P",
        426953719,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "cd0ba4a12ba9b2a7b312c1a79db9770bf6c14820",
        "Game de Hakken!! Tamagotchi",
        "(Japan) (SGB)",
        "DMG-BTAJ",
        4022399590,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "a4e74f4749007fbeb0e2566726df8fef336fc7fa",
        "Trick Boarder",
        "(USA)",
        "CGB-BTBE",
        3898006589,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "db292a8b9201e0cc0d0212a18af0269533937bac",
        "Trickboarder GP",
        "(Japan)",
        "CGB-BTBJ",
        829685911,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "d354c34b83bf41c5aacf3daa596cf73a5828fd7e",
        "Trick Boarder",
        "(Europe)",
        "CGB-BTBP",
        4024695908,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "36e565549b7ab01e62a004f4d441422d89283244",
        "TOCA Touring Car Championship",
        "(USA, Europe)",
        "CGB-BTCE",
        3037301035,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "3dd86b6ce57741654383001fc2b156bfbc162ea5",
        "Test Drive Cycles",
        "(USA)",
        "CGB-BTDE",
        3811291247,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "cba37cc5ac516cb8bf6e9bad68133202bd88ed69",
        "Balloon Kid",
        "(USA, Europe)",
        "DMG-BTE",
        3568719340,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "b138cd95099739c213912375e9b05698d4bdfc19",
        "Test Drive 2001",
        "(USA)",
        "CGB-BTEE",
        3146337991,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "6b0bde35eb0d8096bf92ed71c17ed8f7f615eb49",
        "Tony Hawk's Pro Skater",
        "(USA, Europe)",
        "CGB-BTFE",
        2374743492,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "b60c76813d286fccee27e3988b3ed49ccd25e876",
        "Tony Hawk's Pro Skater 2",
        "(USA, Europe)",
        "CGB-BTGE",
        740804127,
        2097152,
        "En",
        "USA, Europe"
   , " " },
    {
        "97bea9e96132a353d9cd85e42f5fa4d9080da41e",
        "Tarzan",
        "(Japan)",
        "CGB-BTHJ",
        4060109171,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "6d99579408806d18ab825792eaa952b5d159c55d",
        "Racin' Ratz",
        "(USA)",
        "CGB-BTIE",
        3599241236,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "cbe6133fbd94cd1d1448b4a0e4469ffa5895c4a1",
        "Tom and Jerry - Mouse Hunt",
        "(USA) (En,Fr,Es)",
        "CGB-BTJE",
        1606860480,
        1048576,
        "En,Fr,Es",
        "USA"
   , " " },
    {
        "52ebda8a2b6e5b7e59b60232bb22329784219055",
        "Tom and Jerry - Mousehunt",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BTJP",
        1041748042,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "e6177f110d1e2960589a32b30d847bcd6d6b8b09",
        "Tom and Jerry - Mousehunt",
        "(Europe) (En,Fr,De,Es,It) (Rev 1)",
        "CGB-BTJP",
        1262834656,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "bcfdd4a5af0eb3c5dfc846bbee716b1e69d7b6b8",
        "Batman Beyond - Return of the Joker",
        "(USA)",
        "CGB-BTKE",
        3006219654,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "22b0b83326f3687c0b42c337cf20615537acdd3f",
        "Batman Beyond - Return of the Joker",
        "(Japan) (NP)",
        "CGB-BTKJ",
        3098671352,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "0e7828793740473e3045b2d8984d9e487fdf09d2",
        "Batman of the Future - Return of the Joker",
        "(Europe) (En,Fr,De)",
        "CGB-BTKP",
        4191531929,
        1048576,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "c08b905604448f15de0a42a24eb87b2d7ac8df37",
        "Metamode",
        "(Japan)",
        "CGB-BTMJ",
        2809064795,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "21fdef5ceb9e24df0da042f7b365f4bebe0f05e8",
        "Thunderbirds",
        "(Europe)",
        "CGB-BTNP",
        3049180879,
        2097152,
        "En",
        "Europe"
   , " " },
    {
        "80b4f9e690a47d79fa66e1d7aa8ee9e025e43bde",
        "Thunderbirds",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BTNX",
        911434532,
        2097152,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "9c4edef42625cff1802c338650cdadca6f88b707",
        "Triple Play 2001",
        "(USA, Europe)",
        "CGB-BTPE",
        1960856583,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "716fbf2f18b05932ef14984957786e2b09ad95ea",
        "Tom and Jerry in - Mouse Attacks!",
        "(USA)",
        "CGB-BTQE",
        953040758,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "e6aeb1825dbc5c2b8f03d40d2aebf2bd522b81ca",
        "Tom and Jerry in - Mouse Attacks!",
        "(USA) (Rev 1)",
        "CGB-BTQE",
        3461130161,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "2a7ecfb02af72daf7010647c8cfb2957377c576d",
        "Tom and Jerry in - Mouse Attacks!",
        "(Europe) (En,Fr,De,Es,It,Nl,Da)",
        "CGB-BTQP",
        2644280564,
        2097152,
        "En,Fr,De,Es,It,Nl,Da",
        "Europe"
   , " " },
    {
        "e8cfb2c40610e6d5925862e992fc93b8d60a2075",
        "Taisen Tsume Shougi",
        "(Japan) (NP) (GB Comp.)",
        "DMG-BTSJ",
        2503019659,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "70482eff2eadfe3a2631903fb820c20bfe26504b",
        "Tintin - Le Temple Du Soleil",
        "(Europe) (En,Fr,De)",
        "CGB-BTTP",
        2988465481,
        1048576,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "cd3e2844c9251f6c2ef9ef4bbd3dc558e39904e3",
        "Tech Deck Skateboarding",
        "(USA, Europe)",
        "CGB-BTUE",
        3229531760,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "5a3a85ae2488c7441f84fb384d513218f1a703a1",
        "MTV Sports - T.J. Lavin's Ultimate BMX",
        "(USA, Europe)",
        "CGB-BTVE",
        2420532143,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "bf9061a660ee0540c54ae78f5b6e3f6bd02171c5",
        "Tweety's High-Flying Adventure",
        "(USA)",
        "CGB-BTWE",
        1310876566,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "7f65afdd893c68e64afe96499f4ef3ff5aa4a885",
        "Titi - Le Tour du Monde en 80 Chats",
        "(France)",
        "CGB-BTWF",
        3461737752,
        1048576,
        "Fr",
        "France"
   , " " },
    {
        "812ca15649d1bcbb676fc1c0afaee2ff436200f5",
        "Tweety Sekaiisshuu - 80 Hiki no Neko o Sagase!",
        "(Japan)",
        "CGB-BTWJ",
        343884410,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "56a114d5d4d2054fcd510243e3c5b14379889f61",
        "Tweety's High-Flying Adventure",
        "(Europe) (En,Fr,De)",
        "CGB-BTWX",
        1935791804,
        1048576,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "6236432e979b1791f1b3ba3f43a5dee92cef26c2",
        "Tweety's High-Flying Adventure",
        "(Europe) (En,Es,It)",
        "CGB-BTWY",
        3399373701,
        1048576,
        "En,Es,It",
        "Europe"
   , " " },
    {
        "609609b121751629265a09dbec90de66f0ef0d24",
        "Keitai Denjuu Telefang - Power Version",
        "(Japan) (GB Comp.)",
        "DMG-BTXJ",
        2338443212,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "fa7f1b576f7698127f316466a08a33dbf8c1bb97",
        "Keitai Denjuu Telefang - Speed Version",
        "(Japan) (GB Comp.)",
        "DMG-BTZJ",
        4216491878,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "2d1055bca38d2195a71ed78eab1cfd0826c3128c",
        "Moorhuhn 3 - ...Es Gibt Huhn!",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BU4P",
        3082879579,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "0b6093b66710a568219b72cd67d02a87b2580f53",
        "Burai Senshi Deluxe",
        "(Japan)",
        "DMG-BUA",
        1511661811,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "0682f8d9d87e86f10a175f0fe27a7367bf67e49e",
        "Dracula - Crazy Vampire",
        "(USA)",
        "CGB-BUAE",
        674710244,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "ecef19da47f7084b58cfd087c62768a79a30ff26",
        "Dracula - Crazy Vampire",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BUAP",
        796585809,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "e3f26d3b56e975117d36a2408a2b6b1340ccf872",
        "Ultimate Fighting Championship",
        "(USA)",
        "CGB-BUCE",
        3589289678,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "2c05d1d2e4c463f86518580684f8cd9c7141bd4b",
        "Ultimate Fighting Championship",
        "(Europe)",
        "CGB-BUCP",
        2460328365,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "7189f52a53c273c0412fead99064aa753528681b",
        "Donald Duck - Daisy o Sukue!",
        "(Japan)",
        "CGB-BUDJ",
        859574860,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "274ea928ad23a5dc3542134d07ceba82ede76cda",
        "Burai Fighter Deluxe",
        "(USA, Europe)",
        "DMG-BUE",
        1015477723,
        65536,
        "En",
        "USA, Europe"
   , " " },
    {
        "61d5cd73880c907e35482e74c7e4313e0456d515",
        "UEFA 2000",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BUEP",
        1261522251,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "f41925920d08b83085ade3dbccdb4597804ddd50",
        "Grinch",
        "(Japan)",
        "CGB-BUGJ",
        1127072625,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "cd3c756b8d6c02e8a4058984c2f66e532c4f60ac",
        "Heroes of Might and Magic II",
        "(USA) (En,Fr,De)",
        "CGB-BUHE",
        1393913165,
        1048576,
        "En,Fr,De",
        "USA"
   , " " },
    {
        "4833eb82bc525b3acb9a087a7e3f7b942dc39fb5",
        "O'Leary Manager 2000",
        "(Europe) (En,Fr,De,Es,It,Nl,Ca)",
        "CGB-BUIP",
        881161754,
        1048576,
        "En,Fr,De,Es,It,Nl,Ca",
        "Europe"
   , " " },
    {
        "0b7f39fff73bcfcace6ec78340450bc056f725e7",
        "Hamunaptra - Ushinawareta Sabaku no Miyako",
        "(Japan)",
        "CGB-BUMJ",
        4282730257,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "8e7b37068e11dce718df5b0a641863b25da6b8bd",
        "Ultimate Paintball",
        "(USA, Europe)",
        "CGB-BUPE",
        1842339810,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "f792c6f959537687be1f782e54a295f39c857361",
        "Rayman - Mister Dark no Wana",
        "(Japan)",
        "CGB-BURJ",
        1071537068,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "71caf10aca9843170724e10661370d9acd27946a",
        "Tiny Toon Adventures - Buster Saves the Day",
        "(USA)",
        "CGB-BUSE",
        2148465469,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "dbd889100fe2a4cc2ddee2ba719953e2354322bc",
        "Tiny Toon Adventures - Buster Saves the Day",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BUSP",
        2057531885,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "8051206f98ca37019184315a07de94ccbffc2918",
        "Captain Buzz Lightyear - Star Command",
        "(Germany)",
        "CGB-BUZD",
        4033685868,
        524288,
        "De",
        "Germany"
   , " " },
    {
        "c2ad1c607b18e3cb1a32c2be796b888c19135385",
        "Buzz Lightyear of Star Command",
        "(USA, Europe)",
        "CGB-BUZE",
        2229443463,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "91e4032b5af0d02a8eab0c99de749e1422e23530",
        "Aventures de Buzz l'Eclair, Les",
        "(France)",
        "CGB-BUZF",
        2216064176,
        524288,
        "Fr",
        "France"
   , " " },
    {
        "9d4abb34c292dc127c6dbc634396b9de73041143",
        "Mr. Driller",
        "(Japan) (NP)",
        "CGB-BV3J",
        905790477,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "eb545bf5d9247fe9e01fbe598efb7b063556f550",
        "Seme COM Dungeon - Drururuaga",
        "(Japan) (GB Comp.)",
        "DMG-BV6J",
        186356364,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "7b36403d13c2ab7fa761d78d843a85a3a537685d",
        "Power Spike - Pro Beach Volleyball",
        "(USA)",
        "CGB-BVBE",
        2924990915,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "fa781a855cfedebd74ad2c78f40ff4a736d0451b",
        "Beach'n Ball",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BVBP",
        2194777889,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "1c154059967df3991ee19965c86112e5deaa898a",
        "Battletoads",
        "(USA, Europe)",
        "DMG-BVE",
        2965583387,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "0f1528512e0e88a23830e4bcfc0dac271307bb97",
        "VIP",
        "(USA) (En,Fr,Es)",
        "CGB-BVIE",
        1131186132,
        1048576,
        "En,Fr,Es",
        "USA"
   , " " },
    {
        "627becd7964c3c428514ece179fbcf9240ab1b03",
        "VIP",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BVIP",
        3688721797,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "113164c7d63fa6af6ba17728aefb2825f6be68d7",
        "Battletoads",
        "(Japan)",
        "DMG-BVJ",
        857536478,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "01dece4d3da9807ffb855004922689d82fb5be42",
        "Maus, Die - Verrueckte Olympiade",
        "(Germany)",
        "CGB-BVOD",
        4245754766,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "4-19058",
        "3D Pocket Pool",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BVPP",
        2363144063,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "b9d8b8dadc9e3f233935280d734d7b6a9b54acbe",
        "Kaept'n Blaubaer - Die verrueckte Schatzsuche",
        "(Germany)",
        "CGB-BVRD",
        2011250110,
        1048576,
        "De",
        "Germany"
   , " " },
    {
        "edfb876285df01243c5aefc8a824622277c16a92",
        "Buffy the Vampire Slayer",
        "(USA, Europe)",
        "CGB-BVSE",
        1452466786,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "5b4d0e0a2e5053a9290dd2f303f72f073ab38002",
        "Wizardry II - Llylgamyn no Isan",
        "(Japan)",
        "CGB-BW2J",
        3715268378,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "378cbe705b2d547b26492600459b47c8b77b9898",
        "Migrain",
        "(Japan)",
        "DMG-BWA",
        3470341880,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "e2522184b2c1158abf00cf2d2b3eafcec3893e6f",
        "Harvest Moon 3 GBC",
        "(USA)",
        "CGB-BWAE",
        2698408983,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "850276e84a0343e3cb78518691396c717857fb39",
        "Bokujou Monogatari 3 GB - Boy Meets Girl",
        "(Japan) (Rev 1)",
        "CGB-BWAJ",
        1974406788,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "b53bed5ca239f8f66ca144b9c33cbc3f118adf1d",
        "Bokujou Monogatari 3 GB - Boy Meets Girl",
        "(Japan)",
        "CGB-BWAJ",
        974697503,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "12b2a3d99c8a01afd94bbb3eb4d4e09ed1b1d17f",
        "Warau Inu no Bouken - Silly Go Lucky!",
        "(Japan)",
        "CGB-BWBJ",
        3913225602,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "7fef1643901f2a153a87793fee3e392d76276d9f",
        "Hands of Time",
        "(USA, Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BWCE",
        4112118979,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "USA, Europe"
   , " " },
    {
        "36be515e667946a2f8d6f5c532d3cd29ac8aa541",
        "Walt Disney World Quest - Magical Racing Tour",
        "(USA, Europe)",
        "CGB-BWDE",
        1455339156,
        2097152,
        "En",
        "USA, Europe"
   , " " },
    {
        "aaf9bdeb220a547564940388fa3318d0a81eb2ee",
        "Walt Disney World Quest - Magical Racing Tour",
        "(Europe) (Fr,De,Es)",
        "CGB-BWDX",
        3846906553,
        2097152,
        "Fr,De,Es",
        "Europe"
   , " " },
    {
        "d39a5154bb14de1269870ad132944aab8da3c506",
        "Brain Bender",
        "(USA)",
        "DMG-BWE",
        4222743201,
        32768,
        "En",
        "USA"
   , " " },
    {
        "f6ab2e49d69d832a57af6efbabc60f4fcb043e25",
        "WWF Betrayal",
        "(USA, Europe)",
        "CGB-BWFE",
        1814609077,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "d912c79946f7f3c4704bb774f142201d3d30b9c1",
        "Wendy - Every Witch Way",
        "(USA, Europe)",
        "CGB-BWGE",
        1254527099,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "7483d5cf07da16adc7863de7d96a8b4dccef9292",
        "Wizardry I - Proving Grounds of the Mad Overlord",
        "(Japan)",
        "CGB-BWIJ",
        2845455324,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "c10db94c5a50e20b1040f8544891256f79fab340",
        "Warlocked",
        "(USA)",
        "CGB-BWLE",
        3483426575,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "ee869f6214283004173b34ac657288fd5420f17c",
        "Watashi no Restaurant",
        "(Japan)",
        "CGB-BWNJ",
        961545199,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "b8355d16b2ce6d04f4095345a88c3fb7153ccf9f",
        "X-Men - Wolverine's Rage",
        "(USA)",
        "CGB-BWOE",
        318511726,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "ea8108d768010c59456e6cfa78993eec9b36ee6d",
        "X-Men - Wolverine's Rage",
        "(Europe)",
        "CGB-BWOP",
        2209176473,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "77c13d4f331f5fc54b132fbfb8ea0642ff2d2ac1",
        "Woody Woodpecker",
        "(USA)",
        "CGB-BWPE",
        3173851019,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "ca767ba917c542efb1f2bb1944a28b9fc966f9ce",
        "Woody Woodpecker",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BWPP",
        638777540,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "e41731a67859deb8f52b63fed72d539c9d411070",
        "Woody Woodpecker Racing",
        "(USA)",
        "CGB-BWRE",
        69521219,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "6b6168cfbbd3d17e2c78c472d64a1e58351bf986",
        "Woody Woodpecker no Go! Go! Racing",
        "(Japan)",
        "CGB-BWRJ",
        4120750745,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "3039f1959c97ff238e6b262bffa7ac7c8ed5c6b0",
        "Woody Woodpecker Racing",
        "(Europe)",
        "CGB-BWRP",
        2968794264,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "791dfd2f751a58b371b7ae06d3ab6d36d2cfcf1f",
        "International Superstar Soccer 2000",
        "(USA)",
        "CGB-BWSE",
        3908727299,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "cba9ebb9f8e96a283afa24354d77135c332a1b0f",
        "World Soccer GB 2000",
        "(Japan)",
        "CGB-BWSJ",
        1735766686,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "74188a073941e57a5130dffb588b9b06e078e479",
        "International Superstar Soccer 2000",
        "(Europe)",
        "CGB-BWSP",
        1474961096,
        2097152,
        "En",
        "Europe"
   , " " },
    {
        "41c7fdef2624c680dd3b06bbf9d0f416ef98cbba",
        "Wild Thornberrys, The - Rambler",
        "(USA)",
        "CGB-BWTE",
        236217803,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "10401dcd9c9dad6dbd61643fc6752a2d996d7caf",
        "Game Boy Wars 3",
        "(Japan)",
        "CGB-BWWJ",
        818045538,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "7d17b53a5e8bef8de774e888df3c667011f88a65",
        "Brain Bender",
        "(Europe)",
        "DMG-BWX",
        3058810717,
        32768,
        "En",
        "Europe"
   , " " },
    {
        "367b97dc20f24d7409c40c50efe574834554eb95",
        "Wetrix",
        "(Europe) (En,Fr,De)",
        "CGB-BWXP",
        2629806653,
        1048576,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "5e2889089e6e7a7a6433b74248a9becd887728ac",
        "Wendy - Der Traum von Arizona",
        "(Germany)",
        "CGB-BWYD",
        3749451964,
        2097152,
        "De",
        "Germany"
   , " " },
    {
        "a2d2d2a2cfac152a4813a51b0dcda25ce4d62191",
        "Boxing",
        "(Japan)",
        "DMG-BXA",
        4018053024,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "207008e903676272a3c0b76885d1128ce1a268ec",
        "X-Men - Mutant Academy",
        "(USA, Europe) (Rev 1)",
        "CGB-BXAE",
        592322204,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "5f139456a2cc93d272e948a746a64ad7a7052ddb",
        "X-Men - Mutant Academy",
        "(USA, Europe)",
        "CGB-BXAE",
        3965876387,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "1588bb01a50ac90c71f067871b89fcae0902865f",
        "X-Men - Mutant Academy",
        "(Japan)",
        "CGB-BXAJ",
        2170727873,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "78b99f1987a2a1831c180e46754ae4524481b10b",
        "Xtreme Wheels",
        "(USA)",
        "CGB-BXCE",
        311715291,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "0ef802b80bfc77ce93b8aacbacfeb750f78ffccd",
        "Xtreme Wheels",
        "(Europe)",
        "CGB-BXCP",
        3774948463,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "edfd653b58c135daff326d7abaa02d179310d554",
        "Heavyweight Championship Boxing",
        "(USA)",
        "DMG-BXE",
        354855275,
        65536,
        "En",
        "USA"
   , " " },
    {
        "44270b94887c099311feeec1b203851b11a12920",
        "Mary-Kate & Ashley - Get a Clue!",
        "(USA, Europe) (GB Comp.)",
        "DMG-BXFE",
        1190831920,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "3397bc832b8d8de58592edbf35217a5340544894",
        "Taxi 3",
        "(France)",
        "CGB-BXIF",
        674797935,
        1048576,
        "Fr",
        "France"
   , " " },
    {
        "d36a80e6f2d7295ff64ef183baf5b3482976c2c5",
        "Lode Runner - Domudomu Dan no Yabou",
        "(Japan) (GB Comp.)",
        "DMG-BXLJ",
        3974805582,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "9f260eadffa27b599f398e63fe831ff16167d87a",
        "X-Men - Mutant Wars",
        "(USA, Europe)",
        "CGB-BXME",
        2451151330,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "3f9acaf3815b891dd69e9d06bdae256e2508ac12",
        "Cruis'n Exotica",
        "(USA)",
        "CGB-BXOE",
        2506287080,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "d147fdb376e816166ffbfb12b989b75d9652c9f0",
        "Rockman X2 - Soul Eraser",
        "(Japan)",
        "CGB-BXRJ",
        395394512,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "b7d182029b948aa147b8cdeec8a041c7a6cb5662",
        "Road Champs - BXS Stunt Biking",
        "(USA, Europe)",
        "CGB-BXSE",
        2563086086,
        1048576,
        "En",
        "USA, Europe"
   , " " },
    {
        "e89fca2bc6d7d3ce9dafc358dbabc3d2341ed5ec",
        "Pocket Monsters - Crystal Version",
        "(Japan)",
        "CGB-BXTJ",
        655118028,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "00f2263b219499a8db45d1302e9cb70b3a94dc0e",
        "Xena - Warrior Princess",
        "(USA, Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BXWE",
        4041096423,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "USA, Europe"
   , " " },
    {
        "7d9654a0019bcfa46f27b506615b991a5f05b395",
        "Little Mermaid II, The - Pinball Frenzy",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BY2P",
        2683054462,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "0687f6115d7362a952ca5830b432b3057675968e",
        "Yu-Gi-Oh! - Das Dunkle Duell",
        "(Germany)",
        "CGB-BY3D",
        1822907296,
        4194304,
        "De",
        "Germany"
   , " " },
    {
        "60a3322343a556b26ca0cfdc09fe8a64b1ed67ee",
        "Yu-Gi-Oh! - Dark Duel Stories",
        "(USA)",
        "CGB-BY3E",
        2151306926,
        4194304,
        "En",
        "USA"
   , " " },
    {
        "b5e4555622596bc7db0422d1101d3895a5303e5b",
        "Yu-Gi-Oh! - Duel des Tenebres",
        "(France)",
        "CGB-BY3F",
        1534245557,
        4194304,
        "Fr",
        "France"
   , " " },
    {
        "7be85ac69444f0c4e63e201f4e3a7b49b206ec18",
        "Yu-Gi-Oh! - Racconti Oscuri",
        "(Italy)",
        "CGB-BY3I",
        2322270536,
        4194304,
        "It",
        "Italy"
   , " " },
    {
        "b7b3e12f8a0a8732f7343347086a8cff5a270f80",
        "Yu-Gi-Oh! Duel Monsters III - Tri Holy God Advant",
        "(Japan)",
        "CGB-BY3J",
        2677914292,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "101004ef52d9a70fd15b1180444ea2e2977d66d7",
        "Yu-Gi-Oh! - Dark Duel Stories",
        "(Europe)",
        "CGB-BY3P",
        864955610,
        4194304,
        "En",
        "Europe"
   , " " },
    {
        "cfb1befa213949918d6283b0c5c7c5943ba92b24",
        "Yu-Gi-Oh! - Duelo en las Tinieblas",
        "(Spain)",
        "CGB-BY3S",
        3692433168,
        4194304,
        "Es",
        "Spain"
   , " " },
    {
        "c83ca886dcd3e1f15ce208c75b324eaa4395f5e8",
        "Yu-Gi-Oh! Duel Monsters 4 - Battle of Great Duelist - Yuugi Deck",
        "(Japan)",
        "CGB-BY4J",
        1298204150,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "fafc6ff38fb80137fab3fa09f5f2bcff77502b87",
        "Yu-Gi-Oh! Duel Monsters 4 - Battle of Great Duelist - Kaiba Deck",
        "(Japan)",
        "CGB-BY5J",
        2765119489,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "cc38b49b5de730e304ae36a673f500a7ed166f59",
        "Yu-Gi-Oh! Duel Monsters 4 - Battle of Great Duelist - Jounouchi Deck",
        "(Japan)",
        "CGB-BY6J",
        697028692,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "5c028b2d83536c187241904acd119bd63b34f204",
        "Fort Boyard",
        "(Europe) (En,Fr,De,Es,It,Nl,Pt)",
        "CGB-BYDP",
        113829130,
        1048576,
        "En,Fr,De,Es,It,Nl,Pt",
        "Europe"
   , " " },
    {
        "25b29c786426f4bc5785e414af8d49fc3eb4200c",
        "Bugs Bunny Crazy Castle 2, The",
        "(USA)",
        "DMG-BYE",
        2842945028,
        131072,
        "En",
        "USA"
   , " " },
    {
        "42cd870dfa13c5c58b8a523ae479424674c72338",
        "Minnie & Friends - Yume no Kuni o Sagashite",
        "(Japan)",
        "CGB-BYKJ",
        3564823927,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "b5960ce6725ce0ea1257f8636ed2def11e19bee8",
        "Planet of the Apes",
        "(USA) (En,Fr,De,Es,It,Nl)",
        "CGB-BYNE",
        2446838986,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "USA"
   , " " },
    {
        "fa735947961301c8333b110f5ac8782bbddbbabe",
        "Planet of the Apes",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "CGB-BYNP",
        2838350713,
        1048576,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "ff15815d43c99c3f6a1e6e7079d77e183ed52ebe",
        "Mizuki Shigeru no Shin Youkaiden",
        "(Japan)",
        "CGB-BYPJ",
        4229894159,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "40-18818",
        "Pokemon - Kristall-Edition",
        "(Germany)",
        "CGB-BYTD",
        1634567646,
        2097152,
        "De",
        "Germany",
        "/pokemon_crystal64.sprite" 
    },
    {
        "39-4767",
        "Pokemon - Crystal Version",
        "(USA)",
        "CGB-BYTE",
        4000272776,
        2097152,
        "En",
        "USA",
        "/pokemon_crystal64.sprite" 
    },
    {
        "38-6354",
        "Pokemon - Crystal Version",
        "(USA, Europe) (Rev 1)",
        "CGB-BYTE",
        861463306,
        2097152,
        "En",
        "USA, Europe",
        "/pokemon_crystal64.sprite" 
    },
    {
        "38-62178",
        "Pokemon - Version Cristal",
        "(France)",
        "CGB-BYTF",
        2274044583,
        2097152,
        "Fr",
        "France",
        "/pokemon_crystal64.sprite" 
    },
    {
        "35-56250",
        "Pokemon - Versione Cristallo",
        "(Italy)",
        "CGB-BYTI",
        3562717241,
        2097152,
        "It",
        "Italy",
        "/pokemon_crystal64.sprite" 
    },
    {
        "25-17140",
        "Pokemon - Edicion Cristal",
        "(Spain)",
        "CGB-BYTS",
        4278873994,
        2097152,
        "Es",
        "Spain",
        "/pokemon_crystal64.sprite" 
    },
    {
        "23-44924",
        "Pokemon - Crystal Version",
        "(Australia)",
        "CGB-BYTU",
        3144538124,
        2097152,
        "En",
        "Australia",
        "/pokemon_crystal64.sprite" 
    },
    {
        "2540da2358eb6d9a3e0e0aefef3e9f97f35cfd10",
        "Bokujou Monogatari GB",
        "(Japan) (SGB) (NP)",
        "DMG-BYWJ",
        69525381,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "0d284272b5ef2c8a68d09b530440ae9fe26285bf",
        "Zoids - Shirogane no Juukishin Liger Zero",
        "(Japan)",
        "CGB-BZ2J",
        1392969121,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "a8f241360422a8660a2d285ea59481458b51ebf4",
        "Goukaku Boy GOLD - Shikakui Atama o Maruku Suru - Zukei no Tatsujin",
        "(Japan)",
        "DMG-BZ4J",
        1201548312,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "148ee7e9a4a2838e468d99fee48cf7687827b566",
        "Zoboomafoo - Playtime in Zobooland",
        "(USA)",
        "CGB-BZBE",
        953751685,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "3e24f33cb58a1c42db7e88a39ecd9551884a00bb",
        "Battle Unit Zeoth",
        "(USA, Europe)",
        "DMG-BZE",
        3866792627,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "9532f477286e2da30c7836920e33248c6e317b29",
        "DX Jinsei Game",
        "(Japan)",
        "CGB-BZGJ",
        2115351517,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "c2f46280f3045ceceb66499dfde12ccd36b53c4b",
        "Zok Zok Heroes",
        "(Japan)",
        "CGB-BZHJ",
        3231686171,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "b36473561629813a51017b99f43516cba6b5f10b",
        "Battle Unit Zeoth",
        "(Japan)",
        "DMG-BZJ",
        2711748215,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "c2168d1c40dbe53e19085810a21a06efffbef5fb",
        "Muteki Ou Tri-Zenon",
        "(Japan)",
        "CGB-BZNJ",
        195787361,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "0ae7745b0f33aa3875696423d0163f3d6bfbca7c",
        "From TV Animation One Piece - Maboroshi no Grand Line Boukenki!",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-BZOJ",
        2805038008,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "67441978d6bf8d33f17972c29de27354306fa961",
        "Zidane - Football Generation",
        "(Europe) (En,Fr,De,Es,It)",
        "CGB-BZSP",
        3916283829,
        1048576,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "006065902620d50e6075da6a9b5a68e24af2a168",
        "Konchuu Fighters",
        "(Japan)",
        "CGB-BZZJ",
        3329592075,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "6922cee39930a278ee3980db849153b9856adfdd",
        "Chibi Maruko-chan 2 - Deluxe Maruko World",
        "(Japan)",
        "DMG-C2J",
        2885628692,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "d643f480b43622404e59ceef17f8ab5af0e0796d",
        "Alfred Chicken",
        "(USA)",
        "DMG-C3E",
        1000094662,
        131072,
        "En",
        "USA"
   , " " },
    {
        "2b78db1d0628ce91ad365006fa726ced3b1c5226",
        "Alfred Chicken",
        "(Japan) (SGB)",
        "DMG-C3J",
        277416335,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "4c60b3333424d5c95ca8e4c10c4470e6e40ca0a8",
        "Alfred Chicken",
        "(Europe)",
        "DMG-C3X",
        1017850073,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "d8f505c7ea530cef9a724a2349e0e4ced04dafe1",
        "Chibi Maruko-chan 4 - Kore ga Nihon Da yo! Ouji-sama",
        "(Japan)",
        "DMG-C4J",
        3847305406,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "01efa5692d4e200ad6addbb4509af3fcb49a2bb7",
        "Cult Master - Ultraman ni Miserarete",
        "(Japan)",
        "DMG-C5J",
        3286991599,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "c1d172b5af60ad15c206eca6945287fb9f3e7d3b",
        "Felix the Cat",
        "(USA, Europe)",
        "DMG-C6E",
        4114579200,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "ce792decb8bab416ac4a065734b4ef381b1a75d3",
        "Cult Jump",
        "(Japan)",
        "DMG-C7J",
        2486196666,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "b9084c984e02f34b1cb34e44590a380ffe52cbad",
        "Incredible Crash Dummies, The",
        "(USA, Europe)",
        "DMG-C8E",
        3625715962,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "6355acbf9cb9697af3f50a80b32428d5d6acfc02",
        "Mortal Kombat - Shinken Kourin Densetsu",
        "(Japan)",
        "DMG-C9A",
        370125295,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "d85ad8e59721217df13773f0cc113ff8839237e2",
        "Mortal Kombat",
        "(USA, Europe)",
        "DMG-C9E",
        2431322409,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "a101af5a9180c8b3b2e83f278370a9258f1ec868",
        "Square Deal - The Game of Two-Dimensional Poker",
        "(USA)",
        "DMG-CAE",
        2340955973,
        65536,
        "En",
        "USA"
   , " " },
    {
        "7f3b840e03ae8d8671b1dc52dc3a31376f652a25",
        "Cadillac II",
        "(Japan)",
        "DMG-CAJ",
        2798223650,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "fa9aceb8603cc8033e0899e71df99a5f7d171e0b",
        "Pocket Battle",
        "(Japan)",
        "DMG-CBJ",
        387007254,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "e046423a87724bc12d3cd62661ac3058f5b0962a",
        "Chachamaru Panic",
        "(Japan)",
        "DMG-CCJ",
        2861695640,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "a3c1ebcd1d21d7b5090f2716c1360e9866f8c8fc",
        "Hiden Inyou Kikouhou - Ca Da",
        "(Japan)",
        "DMG-CDJ",
        2452942507,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "07994345c8718e2ced9952ed197b1ef210f4638f",
        "Caesars Palace",
        "(USA) (Rev 1)",
        "DMG-CEE",
        2276054533,
        131072,
        "En",
        "USA"
   , " " },
    {
        "495427bac3fe69b51685ab34b8b713524759bc95",
        "Caesars Palace",
        "(USA)",
        "DMG-CEE",
        3656974761,
        131072,
        "En",
        "USA"
   , " " },
    {
        "d246e220c8edad7346f42867437a205689d1f808",
        "Caesars Palace",
        "(Japan)",
        "DMG-CEJ",
        698996,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "dac288ab000978be194f06892b47fac3f7d41272",
        "Caesars Palace",
        "(Europe) (En,Fr,De,Es,It)",
        "DMG-CEX",
        2025797171,
        131072,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "e964bf30510dd9971e32f3460a67750df3d76135",
        "Shinseiki GPX - Cyber Formula",
        "(Japan)",
        "DMG-CFJ",
        1924411047,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "06ba1095764f60dade955ef9a2903b3d98800ebe",
        "Card Game",
        "(Japan)",
        "DMG-CGA",
        3495906448,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "e97a7013589d2802e73c23e4eeb051fa253e832b",
        "Ninja Boy 2",
        "(USA, Europe)",
        "DMG-CHE",
        1690332564,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "785511779c1e22425103503909d281f853de2000",
        "Super Chinese Land 2",
        "(Japan)",
        "DMG-CHJ",
        2286978179,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "3ec44b1bbee61019149c288815793efc2cf46059",
        "Yoshi's Cookie",
        "(USA, Europe)",
        "DMG-CIE",
        1966302994,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "4002305fcad713453e27444a957dd75cd2d9bee6",
        "Yoshi no Cookie",
        "(Japan)",
        "DMG-CIJ",
        2742179326,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "6013244dbb3481b2d1bfe2d5c5a549a9c1dddcb0",
        "Castelian",
        "(USA)",
        "DMG-CJE",
        4074191332,
        32768,
        "En",
        "USA"
   , " " },
    {
        "c81456adb206707afe08ead0820e1cfe124fc978",
        "Kyoro-chan Land",
        "(Japan)",
        "DMG-CJJ",
        3780695259,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "ca3cf0c287429315753b28d9c423a3c2e94ea177",
        "Castelian",
        "(Europe)",
        "DMG-CJX",
        796206084,
        32768,
        "En",
        "Europe"
   , " " },
    {
        "5a7ecfdcc9c584580adf61c3904f47a4a8531cdc",
        "Pachio-kun - Puzzle Castle",
        "(Japan)",
        "DMG-CKJ",
        915458460,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "028fbd6e10692c3cf8272cee44c2e0b0ef74a602",
        "Ninja Boy",
        "(USA, Europe)",
        "DMG-CLE",
        1507058349,
        65536,
        "En",
        "USA, Europe"
   , " " },
    {
        "54ae863a1e3f901d315c99de0d91a62ed13c241c",
        "Super Chinese Land",
        "(Japan)",
        "DMG-CLJ",
        58562154,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "090382535933eb68aaafa34524bfc06e7dfc27ff",
        "Chibi Maruko-chan - Okozukai Daisakusen!",
        "(Japan)",
        "DMG-CMJ",
        3937498623,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "31a60b6d60b96a1ae91a5d93c98a4f5cdb0838a4",
        "Operation C",
        "(USA)",
        "DMG-CNE",
        784056750,
        131072,
        "En",
        "USA"
   , " " },
    {
        "529fd1f12c3b84be4d987b42e31587bc0051b9a6",
        "Contra",
        "(Japan) (En)",
        "DMG-CNJ",
        3454459413,
        131072,
        "En",
        "Japan"
   , " " },
    {
        "3a886bc77ed1b501bca97efab57355e0638cb338",
        "Probotector",
        "(Europe)",
        "DMG-CNX",
        3383543028,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "808bffd3a2298b3991183afc42933a073d6f5358",
        "Crayon Shin-chan 2 - Ora to Wanpaku Gokko Dazo",
        "(Japan)",
        "DMG-COJ",
        3396209632,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "c56fc5aec3f80e8f370962e30cc061f381f5021e",
        "RoboCop",
        "(Japan)",
        "DMG-CPA",
        114726923,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "1ed8a529ccaa75b0c4f44d8fe961953fe82ec60e",
        "RoboCop",
        "(USA)",
        "DMG-CPE",
        1619571750,
        131072,
        "En",
        "USA"
   , " " },
    {
        "4cd8f26f0b50474951cb776e31c89ba3ed7b522d",
        "RoboCop",
        "(USA, Europe) (Rev 1)",
        "DMG-CPE",
        1658154033,
        65536,
        "En",
        "USA, Europe"
   , " " },
    {
        "d311dd2eed11ca69696eed8473170acc9d97ff4c",
        "Crystal Quest",
        "(USA)",
        "DMG-CQE",
        1362103549,
        32768,
        "En",
        "USA"
   , " " },
    {
        "eae76889d745013881b98cc142068a7c1882f24b",
        "Sanrio Carnival",
        "(Japan)",
        "DMG-CSJ",
        1222159226,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "809703b8546006ff548e2018b2653b102224eba1",
        "Cosmo Tank",
        "(USA)",
        "DMG-CTE",
        779517221,
        131072,
        "En",
        "USA"
   , " " },
    {
        "40dc25a7eec3970b30061ac047db6be8c3c339d2",
        "Cosmo Tank",
        "(Japan)",
        "DMG-CTJ",
        2159156721,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "2618cb91dae5a1731ebbdffb59029ddc0f417b2a",
        "Spot - Cool Adventure",
        "(Japan)",
        "DMG-CUA",
        1428837305,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "9ed34aa71b62a581df9484c23ce9bb66cfc18add",
        "Spot - The Cool Adventure",
        "(USA)",
        "DMG-CUE",
        1995863080,
        131072,
        "En",
        "USA"
   , " " },
    {
        "ce08a4bcd414e02101ced25b79b0973d21a03324",
        "McDonaldland",
        "(Europe)",
        "DMG-CUX",
        2178837523,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "ce2e96d6e8d7d0558538f9cc2290cb7f9fc4dd12",
        "Castlevania - The Adventure",
        "(USA)",
        "DMG-CVE",
        560884385,
        65536,
        "En",
        "USA"
   , " " },
    {
        "a79fe6b84eb21783ddf18815ea6cd56eee29f825",
        "Dracula Densetsu",
        "(Japan)",
        "DMG-CVJ",
        2740690677,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "13cba26c41b30490f4f32ea91026e07602108358",
        "Castlevania - The Adventure",
        "(Europe)",
        "DMG-CVX",
        1769456229,
        65536,
        "En",
        "Europe"
   , " " },
    {
        "9eb94c8d71d0c9b686875f75770c72482459d3de",
        "Castlevania II - Belmont's Revenge",
        "(USA, Europe)",
        "DMG-CWE",
        2289420542,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "8ecea8348ef3d4dafc4ccaa805ada76b2405c45a",
        "Dracula Densetsu II",
        "(Japan)",
        "DMG-CWJ",
        1971498516,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "f3cf73574d80ec04ec6a9be6512b151b385be3b0",
        "Chiki Chiki Machine Mou Race",
        "(Japan)",
        "DMG-CXJ",
        2118124618,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "a0d8af63795042c3c6ba0a75c1c572952b0f2f18",
        "Super Bikkuriman - Densetsu no Sekiban",
        "(Japan)",
        "DMG-CYJ",
        3252789313,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "6aabaed725095c3f8da001d661f4e5ec09b58576",
        "Centipede",
        "(USA, Europe) (Accolade)",
        "DMG-CZE",
        609942748,
        32768,
        "En",
        "USA, Europe"
   , " " },
    {
        "475ba0d96e7a42f97b3d9cf7523c74a00986a97b",
        "Double Dragon II",
        "(USA, Europe)",
        "DMG-D2E",
        1536615540,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "6a4f1a6088c6a32f2a92362f3067294b8f85f596",
        "SD Sengokuden 3 - Chijou Saikyou Hen",
        "(Japan)",
        "DMG-D3J",
        2086748780,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "f0ac55f147d20e9e993cd14d843cc46bd350112a",
        "Bram Stoker's Dracula",
        "(USA, Europe)",
        "DMG-D4E",
        13441142,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "baedfac469eef3801cb4f0812da531e2e4205b67",
        "Battle Dodge Ball",
        "(Japan)",
        "DMG-D6J",
        2844934848,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "ea24c14396f167177518910a4807695cac54204d",
        "DuckTales 2",
        "(USA)",
        "DMG-D7E",
        2974896285,
        131072,
        "En",
        "USA"
   , " " },
    {
        "197024018ed3d973eea657c678473dbce324846e",
        "DuckTales 2",
        "(Europe)",
        "DMG-D7X",
        379257025,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "502d6598e450a617b0f62178793cbfa24a7fb447",
        "Pinball Dreams",
        "(USA, Europe)",
        "DMG-D8E",
        1839666147,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "aa5e9cac6e18aee2d198b7efefc601e6a5d9e03e",
        "Daiku no Gen-san - Robot Teikoku no Yabou",
        "(Japan)",
        "DMG-D9J",
        1887542787,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "42e24fbc1adfaf3151192d5598903db47bad17df",
        "Honoo no Toukyuuji - Dodge Danpei",
        "(Japan)",
        "DMG-DAJ",
        3126417559,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "5f8adc2d82cf04634f269433c12f3549ac09faa4",
        "Peke to Poko no Daruman Busters",
        "(Japan)",
        "DMG-DBJ",
        2792315906,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "29-44200",
        "Darkwing Duck",
        "(Germany)",
        "DMG-DCD",
        392961954,
        131072,
        "De",
        "Germany"
   , " " },
    {
        "29-24081",
        "Darkwing Duck",
        "(USA)",
        "DMG-DCE",
        596350534,
        131072,
        "En",
        "USA"
   , " " },
    {
        "29-45859",
        "Darkwing Duck",
        "(Spain)",
        "DMG-DCS",
        2715075908,
        131072,
        "Es",
        "Spain"
   , " " },
    {
        "29-21125",
        "Darkwing Duck",
        "(Europe)",
        "DMG-DCX",
        3197590351,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "7c69944401e43a345057f42a3231431d105c6acf",
        "Double Dragon",
        "(Japan)",
        "DMG-DDA",
        2690932362,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "a895a045d756896edfa3993072f3d2622bdf2aa0",
        "Double Dragon",
        "(USA, Europe)",
        "DMG-DDE",
        1084800786,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "8213873d33ce9bc44a2b1baf7575808010c57303",
        "Doraemon - Taiketsu HimitsuDougu!!",
        "(Japan)",
        "DMG-DEJ",
        2753897635,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "98e52f64cd7024f68ced363a4a13a7b54a92ce61",
        "Kid Dracula",
        "(USA, Europe)",
        "DMG-DFE",
        4067595447,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "9506d35f7144560cd471863a9ab047447f8cf38e",
        "Akumajou Special - Boku Dracula-kun",
        "(Japan)",
        "DMG-DFJ",
        3895677848,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "42cbf76afe35496795d05009054b01adb7cf1b8c",
        "Dungeon Land",
        "(Japan)",
        "DMG-DGJ",
        297683781,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "d2ad7a25252044ef4cdda6a11160e5522aa6ba61",
        "Dead Heat Scramble",
        "(Japan)",
        "DMG-DHA",
        2821725149,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "af489fb6ea6bc7e9ddab7a0a0553ccc0f53fee50",
        "Dead Heat Scramble",
        "(USA)",
        "DMG-DHE",
        2654877270,
        65536,
        "En",
        "USA"
   , " " },
    {
        "51f99b44aea9c4445a1ad20aa01444ff7f851656",
        "Daiku no Gen-san - Ghost Building Company",
        "(Japan)",
        "DMG-DIJ",
        2651663173,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "a1cbd328b96e206f5d8fa67e6692232842d26223",
        "Hammerin' Harry",
        "(Europe)",
        "DMG-DIX",
        2668212200,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "91fa5a8400b3ac620684fce00c573e7cd7ae9053",
        "Dr. Franken II",
        "(USA, Europe) (En,Fr,De,Es,It,Nl,Sv)",
        "DMG-DJX",
        224544573,
        262144,
        "En,Fr,De,Es,It,Nl,Sv",
        "USA, Europe"
   , " " },
    {
        "6eb2fe970db3a1e683df79c414e679df60edbc87",
        "Dick Tracy",
        "(USA)",
        "DMG-DKE",
        2735257707,
        131072,
        "En",
        "USA"
   , " " },
    {
        "86d9f0920217e837d2c5256daef36f3be5e5e2d8",
        "Dragon's Lair",
        "(Japan)",
        "DMG-DLA",
        1942590624,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "4fc2c64aa7fb10fdf962a7f277cd8f999baac2f6",
        "Dragon's Lair - The Legend",
        "(USA)",
        "DMG-DLE",
        2050536899,
        131072,
        "En",
        "USA"
   , " " },
    {
        "cb859b9fdb45311a23f0c3a107befb5ba8771c91",
        "Dragon's Lair - The Legend",
        "(Europe)",
        "DMG-DLX",
        10569865,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "847f8ca96645bd11b145e6e8bf6ae4eb4cd50733",
        "Doraemon 2 - Animal Planet Densetsu",
        "(Japan)",
        "DMG-DMJ",
        2218513852,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "f99980ea933198c58789634eb82b567445e0821f",
        "Darkman",
        "(USA, Europe)",
        "DMG-DNE",
        4286942633,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "0ee54b8de47487e49e056f7f210c0d7be2977e45",
        "Dodge Boy",
        "(Japan)",
        "DMG-DOJ",
        4119708504,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "65fec4e788546b7df2b252f78e6295b813c250cd",
        "Dropzone",
        "(Europe)",
        "DMG-DPX",
        890925687,
        32768,
        "En",
        "Europe"
   , " " },
    {
        "4056f479815cb0630b82e16fff3060e9ba9a17d4",
        "Dragon Slayer Gaiden - Nemuri no Oukan",
        "(Japan)",
        "DMG-DQJ",
        282901955,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "109265bf0f97de5e0c523a887886d0a39fe69422",
        "Dragon Tail",
        "(Japan)",
        "DMG-DRA",
        487675311,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "a649029dec9d4a854374c6694a9277f7cd369b9e",
        "Nail 'n Scale",
        "(USA, Europe)",
        "DMG-DRE",
        1153096631,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "fbc83b37a450035f48175728f7a30fedc77137d6",
        "Dragon Slayer I",
        "(Japan)",
        "DMG-DSA",
        814359653,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "b1b85da65f4186d2b861dc3123d671944833c3c1",
        "DuckTales",
        "(USA)",
        "DMG-DTE",
        733721933,
        65536,
        "En",
        "USA"
   , " " },
    {
        "f30281f3d8b29275ebc06c1777422c7ef903d89a",
        "DuckTales",
        "(Japan)",
        "DMG-DTJ",
        1532236021,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "0d5a12407307e2f80dd938a3af66c34cb0f0cd63",
        "DuckTales",
        "(Europe)",
        "DMG-DTX",
        2892268508,
        65536,
        "En",
        "Europe"
   , " " },
    {
        "f2686ee7d4607bdd7b0a9b17b76194f8cd704048",
        "Downtown - Nekketsu Koushinkyoku - Dokodemo Daiundoukai",
        "(Japan)",
        "DMG-DUJ",
        2186353295,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "b55b540aa48f51ebfa104fabf2fd387b2f438722",
        "Double Yakuman II",
        "(Japan)",
        "DMG-DVJ",
        2748807765,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "73205562d71e114a5c4a6f8dfa46a1cadf9231f7",
        "Double Dribble - 5 on 5",
        "(USA)",
        "DMG-DWE",
        3995628701,
        131072,
        "En",
        "USA"
   , " " },
    {
        "3c267b9771391df5103c88f937832847213727cd",
        "Konamic Basket",
        "(Japan)",
        "DMG-DWJ",
        2385933399,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "3fd80bafdce9db1c22ed463c4e27eb94c1778535",
        "Hyper Dunk",
        "(Europe)",
        "DMG-DWX",
        47226083,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "9cd600fd9b6104a4d26744c66b18dd2fd7ff33e5",
        "Double Dragon 3 - The Arcade Game",
        "(USA, Europe)",
        "DMG-DXE",
        4237757167,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "f2a2ffe38e0f3f470441ea2679ed1988ce05df8b",
        "Dig Dug",
        "(USA)",
        "DMG-DYE",
        1819550840,
        131072,
        "En",
        "USA"
   , " " },
    {
        "029a599570f84f62d8b368bd393cfd65daedf64e",
        "Dig Dug",
        "(Europe)",
        "DMG-DYX",
        184092096,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "aa2cf752ea20cc9603da3fb6185d0d3f1aa32388",
        "DX Bakenou Z",
        "(Japan) (Rev 1)",
        "DMG-DZJ",
        4208972022,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "bc370efdba54f90a96ddcb345a071c195de861c6",
        "DX Bakenou Z",
        "(Japan)",
        "DMG-DZJ",
        1056106531,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "6f439c2294c76bbdd784d5c9fcb9104417bf5c16",
        "Speedball 2 - Brutal Deluxe",
        "(USA, Europe)",
        "DMG-E2E",
        4245086765,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "9c5b4745d2b0396b1a59f78a88299c5d98fd822c",
        "Franky, Joe & Dirk - On the Tiles",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "DMG-E5X",
        3405099890,
        131072,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "3f66154e64f71dc275a750bcccbccb6c957f785b",
        "Metal Masters",
        "(USA)",
        "DMG-E6E",
        3211290332,
        131072,
        "En",
        "USA"
   , " " },
    {
        "f9cecf31f50fc6f829d4b2dd4f855ba441205227",
        "Elevator Action",
        "(Japan)",
        "DMG-EAA",
        3199044324,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "c2eb6383ae6544815003c62e95788eeb0cac8e72",
        "Elevator Action",
        "(USA, Europe)",
        "DMG-EAE",
        3075057959,
        65536,
        "En",
        "USA, Europe"
   , " " },
    {
        "c7404766ba47762885e4c556bd75d24e4ce290e0",
        "Star Wars - The Empire Strikes Back",
        "(USA)",
        "DMG-EBE",
        2701129959,
        131072,
        "En",
        "USA"
   , " " },
    {
        "42bee07b0eb0ba4fe9f05189fea43af267848ddd",
        "Star Wars - The Empire Strikes Back",
        "(USA, Europe)",
        "DMG-EBX",
        604788893,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "0f5b5a749906f9ae40d2bca062a290e967af6fb9",
        "X",
        "(Japan)",
        "DMG-ECJ",
        1977733736,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "f488328280bd975cab347193467b530ede0eebdb",
        "Tiny Toon Adventures - Wacky Sports",
        "(USA)",
        "DMG-EDE",
        3549449193,
        131072,
        "En",
        "USA"
   , " " },
    {
        "e56cca829c329200c609f11755295a9e92aee7d4",
        "Tiny Toon Adventures 3 - Dokidoki Sport Festival",
        "(Japan)",
        "DMG-EDJ",
        1067518257,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "c4b78e83ae20311b713cb55fb0a0ae6e6095a9df",
        "Tiny Toon Adventures - Wacky Sports",
        "(Europe)",
        "DMG-EDX",
        3610361250,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "7869b5f6f421143741774304c360aebb3f8ae2da",
        "Wario Blast Featuring Bomberman!",
        "(USA, Europe) (SGB)",
        "DMG-EEE",
        2457556897,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "7b90de1bf7e3202f864ccad64ac50be791679c76",
        "Bomberman GB",
        "(Japan) (SGB)",
        "DMG-EEJ",
        2486402390,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "8810aa1ed905276ccf7d6c5d10af57b1e03bd82d",
        "Flash, The",
        "(USA, Europe)",
        "DMG-EFE",
        1844125446,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "c26c112cdaf53efb9064e6cee3c474f2cc934782",
        "F-15 Strike Eagle",
        "(USA, Europe)",
        "DMG-EGX",
        73264780,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "db3ad1196c4e65eed3e72e7efc4461ba39e3f1e6",
        "Tetris 2",
        "(USA)",
        "DMG-EHE",
        2856984283,
        131072,
        "En",
        "USA"
   , " " },
    {
        "b6490b286d3979d08a7a4e4513f02512514eddca",
        "Tetris Flash",
        "(Japan) (SGB)",
        "DMG-EHJ",
        2650583941,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "38e0dc0d70f6ef0a8b3669c8e415bc3e238cbf3b",
        "Tetris 2",
        "(USA, Europe) (SGB)",
        "DMG-EHX",
        3927550425,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "b8ad71dec93685ab529a4dd0fc2a07e8de59021f",
        "Tetris 2",
        "(USA, Europe) (Rev 1) (SGB)",
        "DMG-EHX",
        633540618,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "4cde2ec645768022a78e4918bba94460cd7cf02b",
        "Higashio Osamu Kanshuu Pro Yakyuu Stadium '92",
        "(Japan)",
        "DMG-EIJ",
        1901435808,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "ac9c964229ad5943d9b174fcce8031f2ae121de9",
        "Simpsons, The - Bart no Jack to Mame no Ki",
        "(Japan)",
        "DMG-EKA",
        2057625787,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "f83b9fa74ab71928baca6e0c01a3f0792472ed4a",
        "Simpsons, The - Bart & the Beanstalk",
        "(USA, Europe)",
        "DMG-EKE",
        1366974796,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "59c32056ee0d74d4bc7363df60f687741de4091b",
        "Bill Elliott's NASCAR Fast Tracks",
        "(USA)",
        "DMG-ELE",
        1909408718,
        131072,
        "En",
        "USA"
   , " " },
    {
        "e91c54f694cd133225b96daf9a4232c96f74246f",
        "Chessmaster, The",
        "(Japan)",
        "DMG-EMA",
        2182417994,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "c816c203e6eb0ddb1e0c2f9688aa27a5b8581898",
        "Chessmaster, The",
        "(USA, Europe)",
        "DMG-EME",
        203238248,
        65536,
        "En",
        "USA, Europe"
   , " " },
    {
        "d559d675c5c3166e68852a21fd589cb1ecdd63f9",
        "Chessmaster, The",
        "(USA) (Rev 1)",
        "DMG-EME",
        1508718348,
        65536,
        "En",
        "USA"
   , " " },
    {
        "93d9354427e8d3432f0a58f844ed813e6979586d",
        "Chessmaster, The",
        "(Europe)",
        "DMG-EMX",
        42282532,
        65536,
        "En",
        "Europe"
   , " " },
    {
        "8159ee62fb84aaeeda20e08c100105bae3aa26fa",
        "Mogura de Pon!",
        "(Japan)",
        "DMG-EPJ",
        3789775519,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "84e7e6dcbe261b971ff9cb2236df4dbe76d37c0c",
        "Tesserae",
        "(USA)",
        "DMG-ERE",
        3346936175,
        32768,
        "En",
        "USA"
   , " " },
    {
        "5c40994b350a37228ad586eecd300f31c027a96f",
        "Tesserae",
        "(Europe) (En,Fr,De,Es,It)",
        "DMG-ERX",
        1738237232,
        32768,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "164b6921e27c410c5f06178a21becae93f8054dc",
        "Miracle Adventure of Esparks - Ushinawareta Seiseki Perivron",
        "(Japan)",
        "DMG-ESJ",
        3014499190,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "a45292aa95f4a6fcd7a69142ba6e8cf4133e6db9",
        "Tenchi o Kurau",
        "(Japan)",
        "DMG-ETJ",
        2835448660,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "096dfb589ae733f714fbd3c49102db0e52d843d1",
        "F-1 Race",
        "(World) (Rev 1)",
        "DMG-F1A",
        2877537648,
        131072,
        "En",
        "World"
   , " " },
    {
        "8ffa7487beffc99af7ce87a45c2db7d6715e7c3d",
        "F-1 Race",
        "(World)",
        "DMG-F1A",
        2119166943,
        131072,
        "En",
        "World"
   , " " },
    {
        "d9b39b376f44b9b99caab529e521d19fbe1dd46d",
        "Fastest Lap",
        "(Japan) (En)",
        "DMG-F2A",
        1495817994,
        131072,
        "En",
        "Japan"
   , " " },
    {
        "daadcbaa0a38e87153f8cf57cb01c50e06b0e8d4",
        "Fastest Lap",
        "(USA)",
        "DMG-F2E",
        2847570037,
        131072,
        "En",
        "USA"
   , " " },
    {
        "682ce0b7adfa287c6d41a0387bf4b2d08c1f4771",
        "Nakajima Satoru - F-1 Hero GB - World Championship '91",
        "(Japan)",
        "DMG-F3J",
        3759062084,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "6d48bafb9198779a432ccea24592c69388285f65",
        "4-in-1 Fun Pak",
        "(USA, Europe)",
        "DMG-F4E",
        235017958,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "3385e956ce09b9d94ef1ddc57adb06f05ba86dd2",
        "4-in-1 Fun Pak",
        "(Japan)",
        "DMG-F4J",
        2264159043,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "7700ff473ab675853f58805e27971de2cf86f4a0",
        "F1 Pole Position",
        "(USA, Europe)",
        "DMG-F5E",
        2864603579,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "8f60e4f6079505af62c701f00abbf58ccd58d992",
        "Nakajima Satoru Kanshuu - F-1 Hero GB '92 - The Graded Driver",
        "(Japan)",
        "DMG-F5J",
        3069117486,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "74796eb9ab88456c0a2f0e61e2c44d76d16c0f84",
        "Fidgetts, The",
        "(Japan)",
        "DMG-F6A",
        4292127203,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "618d850a659e076a3f5e23616c303f90a2dc206e",
        "Fidgetts, The",
        "(USA, Europe) (En,Fr,De,Es,It,Nl,Sv)",
        "DMG-F6X",
        3019718198,
        262144,
        "En,Fr,De,Es,It,Nl,Sv",
        "USA, Europe"
   , " " },
    {
        "f9ea8c72b5d80d9300ac46c042d3791a591488bf",
        "Super Street Basketball",
        "(Japan)",
        "DMG-F7A",
        2724812191,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "f0431836c64760896484e918acb2109ce4319522",
        "Fire Fighter",
        "(Europe)",
        "DMG-F8X",
        3295395145,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "156-31681",
        "4-in-1 Funpak - Volume II",
        "(USA, Europe)",
        "DMG-F9E",
        25905666,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "57f458a76c5588ca163fb1b6888d13fb3f008d7a",
        "Faceball 2000",
        "(USA)",
        "DMG-FAE",
        2106133712,
        131072,
        "En",
        "USA"
   , " " },
    {
        "4d752109ec4e7fc7c140a8f76f3c08818540aecb",
        "F1 Boy",
        "(Japan)",
        "DMG-FBJ",
        4207015227,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "360d30507610c2a0b72f7b01eeced486bd29f27b",
        "Sunsoft Grand Prix",
        "(Europe)",
        "DMG-FBX",
        2354966093,
        65536,
        "En",
        "Europe"
   , " " },
    {
        "27fa2b0a20688c1b4095652b9a1dbdc30883505e",
        "Fleet Commander VS.",
        "(Japan)",
        "DMG-FCJ",
        1908355670,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "467c8d022f0a6127326aa4f338a69bb541305311",
        "Famista 2",
        "(Japan)",
        "DMG-FDJ",
        605711948,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "04f9d9687b5261ea6de451bddc58874e0e387a72",
        "Flintstones, The - King Rock Treasure Island",
        "(USA, Europe)",
        "DMG-FEE",
        1350730448,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "87e55be296bb1ec5ff00d20fe740f61a9cdecfd7",
        "Mystic Quest",
        "(Germany)",
        "DMG-FFD",
        55687590,
        262144,
        "De",
        "Germany"
   , " " },
    {
        "d1975d143d63e16e85422ed7d7fde128771cf50d",
        "Final Fantasy Adventure",
        "(USA)",
        "DMG-FFE",
        415730490,
        262144,
        "En",
        "USA"
   , " " },
    {
        "9f83a2867045e5e75ab02b7271201ccc3c9c5230",
        "Mystic Quest",
        "(France)",
        "DMG-FFF",
        3068212399,
        262144,
        "Fr",
        "France"
   , " " },
    {
        "df1539bf066c246cbf38af5b02b7ab84a3fafb50",
        "Seiken Densetsu - Final Fantasy Gaiden",
        "(Japan)",
        "DMG-FFJ",
        3614556598,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "6b940656fd92071e3aa8608863b6dac62d1c2966",
        "Mystic Quest",
        "(Europe)",
        "DMG-FFX",
        1473862802,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "77e4e7f663f89d6880f8dfd1161d78dcbfbd8193",
        "InfoGenius Productivity Pak - Berlitz French Translator",
        "(USA, Europe)",
        "DMG-FHE",
        2152226778,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "66858ebb01a4c966fc112a0fb5ab2383ababf60d",
        "Final Reverse",
        "(Japan)",
        "DMG-FIJ",
        3913967938,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "8b786a73cee87fd6b518f9c5d010dd56c02836f4",
        "Family Jockey",
        "(Japan)",
        "DMG-FJJ",
        3605992528,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "3e365d85173867ecaa811b7fe32adf7d2b06c4b1",
        "Krusty World",
        "(Japan)",
        "DMG-FKA",
        33278454,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "2954f3676050e8e247db4c6a8cae4a93ed90baa3",
        "Krusty's Fun House",
        "(USA, Europe)",
        "DMG-FKE",
        485339457,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "e70c9ee2d0c13cf41be5a71a8fc5b84bf7d246b3",
        "Play Action Football",
        "(USA)",
        "DMG-FLE",
        2554113862,
        131072,
        "En",
        "USA"
   , " " },
    {
        "86a37aeafcf9bdcf265b5f0e1a4ac124ea8ebb1c",
        "Shin Nihon Pro Wrestling - Toukon Sanjuushi",
        "(Japan)",
        "DMG-FMJ",
        2829332499,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "b05d7c06c5939adb2a213c29ab41bb06704e7cf8",
        "Dr. Franken",
        "(Japan)",
        "DMG-FNA",
        1023719655,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "c73afe4ccee9c8f3bb4a17334b485c2e529133a7",
        "Dr. Franken",
        "(USA)",
        "DMG-FNE",
        3557373787,
        131072,
        "En",
        "USA"
   , " " },
    {
        "2b157118213c32d575834755fc34ca82116f93ca",
        "Dr. Franken",
        "(Europe) (En,Fr,De,Es,It,Nl,Sv)",
        "DMG-FNX",
        4047134904,
        262144,
        "En,Fr,De,Es,It,Nl,Sv",
        "Europe"
   , " " },
    {
        "fca0ad4b3a67aac5a26b6515079083af1edfdb9a",
        "Titus the Fox",
        "(USA, Europe)",
        "DMG-FOE",
        2169479494,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "bd6a0192d9897d6ddc198635d326abee5af70517",
        "Flipull - An Exciting Cube Game",
        "(Japan)",
        "DMG-FPA",
        428807293,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "9293d553669c8ff693bdba9187d806f0a3cb88a9",
        "Flipull",
        "(USA)",
        "DMG-FPE",
        2733837131,
        32768,
        "En",
        "USA"
   , " " },
    {
        "7-45753",
        "Suzuki Aguri no F-1 Super Driving",
        "(Japan)",
        "DMG-FQJ",
        3894842073,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "c29056d0ffecb7280a5b2eec2d2d455b1b2c5d5d",
        "Extra Bases",
        "(USA)",
        "DMG-FSE",
        425756225,
        131072,
        "En",
        "USA"
   , " " },
    {
        "b75c4ae0cd978014a443f1b73ab44acc482c9c2e",
        "Famista",
        "(Japan)",
        "DMG-FSJ",
        1027343774,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "be06cd8479d1f05dd992b3478fb4676c2571e13a",
        "NFL Football",
        "(USA)",
        "DMG-FTA",
        1279071496,
        32768,
        "En",
        "USA"
   , " " },
    {
        "9119bd5128bd461324fe0a74365c507ab26701fd",
        "Funny Field",
        "(Japan)",
        "DMG-FUA",
        3218635428,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "b6405dd0bd27cb5a709cc051259ea43562feff3b",
        "Dexterity",
        "(USA, Europe)",
        "DMG-FUE",
        1704862339,
        65536,
        "En",
        "USA, Europe"
   , " " },
    {
        "8b7bbeabdbf4968acd62550d34d4e064de541dc1",
        "Family Jockey 2 - Meiba no Kettou",
        "(Japan)",
        "DMG-FVJ",
        52815657,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "76668eea4f7e1bb9b7028c2ba947d0ea79a41b0e",
        "Knight Quest",
        "(USA)",
        "DMG-FWE",
        3746624631,
        131072,
        "En",
        "USA"
   , " " },
    {
        "711e22b015f9b61312f68b0432f8efaf5d752a11",
        "Knight Quest",
        "(Japan)",
        "DMG-FWJ",
        3337768239,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "75aaaff09c4d9060565a6568ba7c27c1e797989d",
        "Out of Gas",
        "(USA)",
        "DMG-FXE",
        459794609,
        131072,
        "En",
        "USA"
   , " " },
    {
        "555c7cef1c4e81dc558dfce631aa01aa36f0c904",
        "World Circuit Series",
        "(USA)",
        "DMG-FZE",
        2009311755,
        131072,
        "En",
        "USA"
   , " " },
    {
        "4a0a94580bb09e3f7a3bea9df40199a91b200462",
        "F-1 Spirit",
        "(Japan)",
        "DMG-FZJ",
        4192241539,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "69f01325249ebe9b3cb406aa3a1302f2ebfa6e41",
        "Spirit of F-1, The",
        "(Europe)",
        "DMG-FZX",
        729371776,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "385ee6245cfeeddb2492255c8b4353c28eb960fc",
        "Gauntlet II",
        "(USA, Europe)",
        "DMG-G2E",
        419945157,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "639fb1c2bd6998625142c23d05bab6277486caab",
        "SD Sengokuden 2 - Tenka Touitsu Hen",
        "(Japan)",
        "DMG-G3J",
        1534881706,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "22c800250188101e38895aa3bfdfcfc8a37afd76",
        "Genki Bakuhatsu Gambaruger",
        "(Japan)",
        "DMG-G4J",
        1646711424,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "72bb4ccfd5ba0345c3804966504c867e37451103",
        "Momotarou Dengeki",
        "(Japan)",
        "DMG-G5J",
        4036703403,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "06b3f14ff725d3bfa08a42d286bb04891e02be09",
        "Speedy Gonzales",
        "(USA, Europe)",
        "DMG-G6E",
        667805910,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "6bc291f5f5e9abe611e35f9e59b613ba1379fe7b",
        "Soreyuke! Speedy Gonzales",
        "(Japan)",
        "DMG-G6J",
        3001459141,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "9121956ad8dbce49fcaf2d238e8635208b1dde9b",
        "Kaijuu Ou Gojira",
        "(Japan)",
        "DMG-G7J",
        3307841898,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "eb4dc23d873a5372095b46004ff779a0f391e177",
        "Gear Works",
        "(USA, Europe)",
        "DMG-G8E",
        2998345814,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "099b11413970762294ad1a0941785fd314b7354c",
        "Super Momotarou Dentetsu II",
        "(Japan)",
        "DMG-G9J",
        3904145695,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "cf838618f3625ec58022dc4933c772e318161137",
        "G Arms - Operation Gundam",
        "(Japan)",
        "DMG-GAJ",
        956662099,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "7f0f34384bdf98b4822ae5a17a3c3746915072e4",
        "Ghostbusters II",
        "(Japan)",
        "DMG-GBA",
        1773232572,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "a4f58eb6d395b77941553bd89a23fc66100278d0",
        "Ghostbusters II",
        "(USA, Europe)",
        "DMG-GBE",
        1478618324,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "9df58eff20667e593599d0b1d560f22163f0fb90",
        "Ganbare Goemon - Sarawareta Ebisumaru",
        "(Japan)",
        "DMG-GCJ",
        858135569,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "e77f915b01fbee046ea822f15d79be9b76eef410",
        "InfoGenius Productivity Pak - Frommer's Travel Guide",
        "(USA)",
        "DMG-GDE",
        1076112949,
        262144,
        "En",
        "USA"
   , " " },
    {
        "7dc07a50200d9eb13d117dbdf5b004c9433e26f1",
        "Gem Gem",
        "(Japan)",
        "DMG-GEJ",
        2789902096,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "0629dfbcf16f96264b9bb8da73574640e30a4f78",
        "Ginga - Card & Puzzle Collection",
        "(Japan) (En,Ja)",
        "DMG-GGA",
        2278581115,
        65536,
        "En,Ja",
        "Japan"
   , " " },
    {
        "417d7860a5e6893b49c072f035ab7cf322b2cb54",
        "Game of Harmony, The",
        "(USA)",
        "DMG-GHE",
        2953267915,
        32768,
        "En",
        "USA"
   , " " },
    {
        "09267740c82518234c9ca7df436d21537c0eedb4",
        "GI King! - Sanbiki no Yosouya",
        "(Japan)",
        "DMG-GIJ",
        353092241,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "5a4c83e847b207f28964bc3fc9a089c9ae5d5100",
        "Jantaku Boy",
        "(Japan)",
        "DMG-GJJ",
        81436731,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "0ecb640c912251bb64db65fba44a355beb732a57",
        "Bonk's Adventure",
        "(USA)",
        "DMG-GKE",
        2815277974,
        262144,
        "En",
        "USA"
   , " " },
    {
        "5fa06192edb15fa9f1930ef9b368257a96cc0985",
        "GB Genjin",
        "(Japan)",
        "DMG-GKJ",
        1761748982,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "b50e233f2cd71f55459a4d0173a69aa992618606",
        "B.C. Kid",
        "(Europe)",
        "DMG-GKX",
        926106598,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "4ae2547b9eb99db791555eaf30f5350ec5f63469",
        "Boggle Plus",
        "(USA)",
        "DMG-GLE",
        1892206489,
        131072,
        "En",
        "USA"
   , " " },
    {
        "eaa8926153a0069f9423a04d88f02cc914fde659",
        "BurgerTime Deluxe",
        "(World)",
        "DMG-GMA",
        2283903561,
        65536,
        "En",
        "World"
   , " " },
    {
        "357e0b813f4de76536f0e571f834d3c3a05fcf62",
        "Top Gun - Guts & Glory",
        "(USA, Europe)",
        "DMG-GNE",
        84328747,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "55e59888a50d28f0487b18bb3471698ace6293d4",
        "Golf",
        "(World)",
        "DMG-GOA",
        1859191683,
        131072,
        "En",
        "World"
   , " " },
    {
        "55c3de990182ccc82bfb0acdbc3b349600e879f7",
        "Mr. Chin's Gourmet Paradise",
        "(USA)",
        "DMG-GPE",
        3493747114,
        65536,
        "En",
        "USA"
   , " " },
    {
        "47-26280",
        "Adventure Island II - Aliens in Paradise",
        "(USA, Europe)",
        "DMG-GQE",
        2016437967,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "6c26849f4113ca277311d97001a28030e4f143ac",
        "Takahashi Meijin no Bouken-jima III",
        "(Japan)",
        "DMG-GQJ",
        4177267521,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "078c10b02b400bf30a6b28ee217e1fd94b8c8419",
        "Gremlins 2 - The New Batch",
        "(World)",
        "DMG-GRA",
        897180311,
        131072,
        "En",
        "World"
   , " " },
    {
        "c1833482c7e2eb9a957765230ab780d73b858861",
        "Soccer",
        "(Japan)",
        "DMG-GSA",
        3193022582,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "b9fd64c43aabb54edca0b4fb19985737e7783268",
        "Football International",
        "(Europe)",
        "DMG-GSX",
        1852151440,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "1ed4e078e2a30376fb13aab6864d78d67a595dcf",
        "Go! Go! Tank",
        "(Japan)",
        "DMG-GTA",
        816724406,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "245d9a28aca441c866995d1f7bff2cdd9cb3184b",
        "Go! Go! Tank",
        "(USA)",
        "DMG-GTE",
        1709157323,
        65536,
        "En",
        "USA"
   , " " },
    {
        "2990b14b6aacdcb3fd2b5811c6a6917b74bd3f7c",
        "Barbie - Game Girl",
        "(USA, Europe)",
        "DMG-GUE",
        2495769823,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "2f00c48331810e9b29cf7a9439f007aac19fe810",
        "Burning Paper",
        "(Japan)",
        "DMG-GVA",
        2298005905,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "2cc788b5e0b8995f7da02a6057ab0ce8f833c7df",
        "Game Boy Wars",
        "(Japan)",
        "DMG-GWJ",
        4124271835,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "7bb28f0094e88f1cb352b4ecbbed4857ba9e4256",
        "God Medicine - Fantasy Sekai no Tanjou",
        "(Japan)",
        "DMG-GXJ",
        2712836792,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "6beb1f815c08a70e44816b57e0b1c0638b49fa1b",
        "Godzilla",
        "(USA, Europe)",
        "DMG-GZE",
        4266623159,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "8102460b1b7ac1c4432bbaf75645554b8cca865d",
        "Gojira-kun - Kaijuu Daikoushin",
        "(Japan)",
        "DMG-GZJ",
        3978315264,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "d90b5391908f68a30297680b8205325e6799d6f8",
        "Jikuu Senki Mu",
        "(Japan)",
        "DMG-H1J",
        1130120855,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "b7e6b124cbeebb7ee35ba2fd4c57fbc1a39b1519",
        "NBA All-Star Challenge 2",
        "(Japan)",
        "DMG-H2A",
        3751653395,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "78df73566f8b522507e899aeddf2dfc71794587e",
        "NBA All-Star Challenge 2",
        "(USA, Europe)",
        "DMG-H2E",
        3262907017,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "9fa8563098a8f996b99d2c1dcdb3479791c1106d",
        "Micro Machines",
        "(USA, Europe)",
        "DMG-H3E",
        545847391,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "787a80a392594ac313fbd89cfd7c34a685985d2f",
        "Robot Poncots - Moon Version",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-H3UJ",
        2275577904,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "d967a1b4f3ce93ae853a7dbac4f27744f61955a0",
        "Championship Pool",
        "(USA)",
        "DMG-H4E",
        4009934518,
        131072,
        "En",
        "USA"
   , " " },
    {
        "ecfbb447cebb2b032dece18c1c3997e0d3eea944",
        "Road Rash",
        "(USA, Europe)",
        "DMG-H5E",
        2297284669,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "b414ae0b49db4ac95dbe83ddb40d71b5b26d892b",
        "Robot Poncots - Comic Bom Bom Special Version",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-H5UJ",
        1252133325,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "c25de27329ef0705d9a4df5aab97d900889bd08b",
        "Hitori de Dekirumon! - Cooking Densetsu",
        "(Japan)",
        "DMG-H7J",
        1507548789,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "f7d1ba19e692602eeb37eab5d9a6dc210485971e",
        "Crayon Shin-chan 4 - Ora no Itazura Daihenshin",
        "(Japan) (SGB)",
        "DMG-H9J",
        936684172,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "c7ff0760b3cf71c8bda4be70160a88ae13bce702",
        "Heiankyou Alien",
        "(Japan)",
        "DMG-HAA",
        146745801,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "cb662ef35eef6351f2d12dde132b07efe66ef8c7",
        "Heiankyo Alien",
        "(USA)",
        "DMG-HAE",
        345357285,
        32768,
        "En",
        "USA"
   , " " },
    {
        "e0f480feb32d2c2bf1bc9e7fee70cf2667b004bc",
        "Bomber Boy",
        "(Japan)",
        "DMG-HBA",
        4019557804,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "7e29f9de5d8cf98551e1f2b3db0b0aa8b17050cd",
        "Atomic Punk",
        "(USA)",
        "DMG-HBE",
        3295807639,
        131072,
        "En",
        "USA"
   , " " },
    {
        "f703ba835e02527149089a66e088077d46bab7f7",
        "Dynablaster",
        "(Europe)",
        "DMG-HBX",
        2524434775,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "a8fcd1f98c8bbf75eefdc9bf68fcfc5c19da331b",
        "Hit the Ice - VHL - The Official Video Hockey League",
        "(USA, Europe)",
        "DMG-HCE",
        746058649,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "c02ebfaf09b078422d49772c1f1807e4f81eb6f8",
        "Daisenryaku",
        "(Japan)",
        "DMG-HDJ",
        3371699600,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "e4cedf73906eddf9a86fa8c6f659d3e32db01ce7",
        "Heracles no Eikou - Ugokidashita Kamigami",
        "(Japan)",
        "DMG-HEJ",
        1334469055,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "5358247a6473d602af28f9fe0f11890d68f53d9d",
        "Pocket Family GB 2",
        "(Japan)",
        "CGB-HF2J",
        707211844,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "a24990232fa6e57b45aac51933a513624a60fbf9",
        "Red October o Oe!",
        "(Japan)",
        "DMG-HFA",
        3269967413,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "9937c59a2c991cac2e1520f5da314fab8cdb3ebc",
        "Pocket Family GB",
        "(Japan) (SGB)",
        "DMG-HFAJ",
        1218002255,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "8b6c38d862b0e0b238a8ac5d29078a11a0e953fe",
        "Hunt for Red October, The",
        "(USA, Europe)",
        "DMG-HFE",
        1516367360,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "f3245700438c983e6b408d9f3c2642ce5414d72e",
        "Hong Kong",
        "(Japan)",
        "DMG-HGJ",
        1524120936,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "484f69b038dcf9f4247486777eb0a05dc40400b6",
        "Capcom Quiz - Hatena no Daibouken",
        "(Japan)",
        "DMG-HHJ",
        2151854021,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "87fa41e6d12dd286bb48eaaea650ea117e845652",
        "Crayon Shin-chan 3 - Ora no Gokigen Athletic",
        "(Japan)",
        "DMG-HJJ",
        614541319,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "a32df3fa86c38e716ec3c03a0303eac8e5974662",
        "Hello Kitty - Pocket Camera",
        "(Japan) (Proto) (SGB)",
        "GBD-HKAJ",
        628806725,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "51328bda83c2bb9775379918f7b2e7b3cfae8349",
        "Fist of the North Star",
        "(USA)",
        "DMG-HKE",
        2592388815,
        131072,
        "En",
        "USA"
   , " " },
    {
        "ca2f4aaf369cb826e5dbe9846caa4b7ba9fdd1f0",
        "Hokuto no Ken - Seizetsu Juuban Shoubu",
        "(Japan)",
        "DMG-HKJ",
        3837066940,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "d90fc104cc64838bcc1fb0c960cf8b12710b083f",
        "Hyper Lode Runner",
        "(World) (Rev 1)",
        "DMG-HLA",
        3014156644,
        32768,
        "En",
        "World"
   , " " },
    {
        "d260bd01e280f0ee683e7efab16e9a5a2534691b",
        "Home Alone",
        "(USA, Europe)",
        "DMG-HME",
        2398913588,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "43dce45d883b95657258be9cad779925f89b8a1d",
        "Home Alone",
        "(Japan)",
        "DMG-HMJ",
        2084516103,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "4f5938de45c9d3d235ffbbcfe5c24f53bbd8a60d",
        "Super Hunchback",
        "(USA)",
        "DMG-HNE",
        440758382,
        131072,
        "En",
        "USA"
   , " " },
    {
        "b4e385abe389897206353141aea963f499f4afb2",
        "Super Hunchback",
        "(Japan)",
        "DMG-HNJ",
        4060117186,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "fa0b22154f23e6244e0c22d1d756fcc48c524cc3",
        "Super Hunchback",
        "(Europe)",
        "DMG-HNX",
        593090078,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "cce8b7f97ba811a42f158649e15f5838e1d03717",
        "Rock'n! Monster!!",
        "(Japan) (SGB)",
        "DMG-HOJ",
        1661114817,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "99b32821f410d2342ffd2571574ba3e3570be39b",
        "Black Bass - Lure Fishing",
        "(USA)",
        "DMG-HPE",
        766761678,
        262144,
        "En",
        "USA"
   , " " },
    {
        "78ea2c4f410a882b9976e8baf827eed0f1398d5b",
        "Hyper Black Bass",
        "(Japan) (En,Ja)",
        "DMG-HPJ",
        3278165343,
        262144,
        "En,Ja",
        "Japan"
   , " " },
    {
        "dec09c3f5c6c708eb185c56565e61f4eeeacadde",
        "Chase H.Q.",
        "(USA, Europe)",
        "DMG-HQE",
        1738824985,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "1b13e955de1e5a595ed915c0bc174aafa53f0963",
        "Taito Chase H.Q.",
        "(Japan)",
        "DMG-HQJ",
        3314267381,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "050023429e14082dbe6cdeef50f563d106b09fd6",
        "Robot Poncots - Star Version",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-HRCJ",
        2995729068,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "4f921cc3d680dad5a047ece2ceb2543ebe4bc9a5",
        "Fighting Simulator 2 in 1 - Flying Warriors",
        "(USA, Europe)",
        "DMG-HRE",
        1382517480,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "3a2937b83ed26b28ef4f3f6950bdeaf4bef08e61",
        "Robopon - Sun Version",
        "(USA) (SGB) (GB Comp.)",
        "DMG-HREE",
        852160273,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "a1aba7da37fe3c6c0c5f5de5e4faf8c8b9f9c011",
        "Robot Poncots - Sun Version",
        "(Japan) (SGB) (GB Comp.)",
        "DMG-HREJ",
        3406528515,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "9bf01b8a1d3d0cb88b420250557be1a367480828",
        "Hiryuu no Ken Gaiden",
        "(Japan)",
        "DMG-HRJ",
        2397658635,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "025c29d9477a7b86cac9b53957f7bc0c9bc478fc",
        "Hook",
        "(USA)",
        "DMG-HSE",
        1734719003,
        131072,
        "En",
        "USA"
   , " " },
    {
        "f12625582d470eec83ce299321eb441ed2ecf01e",
        "Hook",
        "(Japan)",
        "DMG-HSJ",
        1658328892,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "276dbfb27cbdc79637ce276934e54c5a602d9295",
        "Hook",
        "(Europe)",
        "DMG-HSX",
        923413551,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "e0a8c8aec4fd39b3ebe30b338a518dfb7f17a279",
        "Hatris",
        "(Japan, USA) (En)",
        "DMG-HTA",
        1983246987,
        65536,
        "En",
        "Japan, USA"
   , " " },
    {
        "44903866baff6709714425a47e4eea1bd35ae639",
        "Humans, The",
        "(USA)",
        "DMG-HUE",
        502458397,
        262144,
        "En",
        "USA"
   , " " },
    {
        "1c2a025a5367c1f82dfd401a8ba940289b26c12a",
        "Humans, The",
        "(Europe) (En,Fr,De,Es,It)",
        "DMG-HUX",
        2577164246,
        262144,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "2a060cf78f81dd37d5dd7a60c986b80f611fe8a9",
        "Taiyou no Tenshi Marlowe - Ohanabatake wa Dai-panic",
        "(Japan)",
        "DMG-HVJ",
        288540347,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "99aca9ee90d3f65d3d648318345c26e788888cca",
        "Hudson Hawk",
        "(USA)",
        "DMG-HWE",
        2919201725,
        131072,
        "En",
        "USA"
   , " " },
    {
        "b45168d47054f26fb5400deb9a992b598b057048",
        "Hudson Hawk",
        "(Japan)",
        "DMG-HWJ",
        481695116,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "026c09b3b964c63e6c2e5d4a40e212e08e2967b6",
        "From TV Animation Slam Dunk - Gakeppuchi no Kesshou League",
        "(Japan) (SGB)",
        "DMG-HXJ",
        2923924079,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "c240fb4f124dd6f5d2a9a49737b869c7b7a0c188",
        "High Stakes Gambling",
        "(USA)",
        "DMG-HYE",
        1796713445,
        131072,
        "En",
        "USA"
   , " " },
    {
        "a4b404dfffabd945cd50c9d32e5d4fbe276a6f57",
        "Stop That Roach!",
        "(USA)",
        "DMG-HZE",
        2249225335,
        131072,
        "En",
        "USA"
   , " " },
    {
        "53b2e5b78baeab07da4af9e703d59289802d5126",
        "Hoi Hoi - Game Boy Ban",
        "(Japan)",
        "DMG-HZJ",
        3267538201,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "55b7fa03bcbe0c67730321ca98822f862ac0ab06",
        "Ikari no Yousai 2",
        "(Japan)",
        "DMG-I2J",
        1833949610,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "2ebf9f1af84e189fd1d847e8d3925e9e54089f53",
        "Mickey Mouse V",
        "(Japan)",
        "DMG-I5J",
        1154286045,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "514e2e3f87dea2eff549750dada5026f4553e1e8",
        "Mickey Mouse V",
        "(Japan) (Rev 1)",
        "DMG-I5J",
        1179560541,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "a4d32560bf62918767a9c34c09e49f3a3482a9d0",
        "Mickey Mouse - Magic Wands!",
        "(USA, Europe) (SGB)",
        "DMG-I5X",
        4031260007,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "f2345ce86d0e62edb64f8b3f514e9140668dda81",
        "Tekkyu Fight! - The Great Battle Gaiden",
        "(Japan)",
        "DMG-IBJ",
        3101331125,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "3eb418a61181c08c7c41a574b01d7b85fddaee10",
        "Shuyaku Sentai Irem Fighter",
        "(Japan)",
        "DMG-IFJ",
        3973455619,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "0ccc90cd1af7e247d5b93b6e6a2fc915375b3cac",
        "Ishida Yoshio Tsumego Paradise",
        "(Japan)",
        "DMG-IGJ",
        3380456138,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "94bb4d1bd5c6f42d32d5c84ad9cfd5a654306fde",
        "Honmei Boy",
        "(Japan)",
        "DMG-IKJ",
        220552537,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "aeed299fc64daca6b34b7c3d9f2afcbbb4a5b054",
        "GB Pachi-Slot Hisshouhou! Jr.",
        "(Japan)",
        "DMG-IPJ",
        2732724457,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "b8fd0b85ba62f061bf50af281a71296d31bd1087",
        "Shippuu! Iron Leaguer",
        "(Japan)",
        "DMG-IRJ",
        2386329109,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "76b5cde497ede34e6fdd187ac4d490620183b06a",
        "Sangokushi - Game Boy Ban",
        "(Japan)",
        "DMG-ISJ",
        2205183634,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "64e02799db5a81efc254a5cfd2a6406c2beb2fa3",
        "Fortified Zone",
        "(USA, Europe)",
        "DMG-IYE",
        3820329854,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "777664df795a93694b9dd954fe852f797a58726f",
        "Ikari no Yousai",
        "(Japan)",
        "DMG-IYJ",
        472621729,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "600806215d96a4a4bf78d2714516acc6ffd3676c",
        "Jungle no Ouja Tar-chan",
        "(Japan) (SGB)",
        "DMG-J3J",
        2522177178,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "5614d8d2d94d295866ee4ba217582ab69cf10cd6",
        "Janshirou II - Sekai Saikyou no Janshi",
        "(Japan)",
        "DMG-J4J",
        672269163,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "1d9ae6d638eb4b7caa78a6916e4267243b6651f0",
        "Indiana Jones and the Last Crusade",
        "(USA, Europe)",
        "DMG-J5E",
        2441712154,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "4287dbb039f90cc6e76acf7c854c25ae3c74cff6",
        "Indiana Jones - Saigo no Seisen",
        "(Japan)",
        "DMG-J5J",
        2401455945,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "4bdaaa6b03304c4fa536dc1333f2d985d663d8e2",
        "Blues Brothers, The - Jukebox Adventure",
        "(Europe)",
        "DMG-J6E",
        4055956253,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "b536490a59318d8584e01bc6fdc0f357b0a213ee",
        "Jungle Book, The",
        "(USA, Europe)",
        "DMG-J7E",
        3058509806,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "105976f757bf29d5dc25101554b840b0bab8d815",
        "Tom to Jerry Part 2",
        "(Japan)",
        "DMG-J8A",
        3709446636,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "c7807361bc1ca31fcb7bbfed6b37a787d67be91a",
        "Tom and Jerry - Frantic Antics!",
        "(USA, Europe)",
        "DMG-J8E",
        3269185962,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "653e20adc11da0664c66786692cd08c9269434f1",
        "Jankenman",
        "(Japan)",
        "DMG-JAJ",
        934531201,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "1269f87beb46ab6ac6006937eec4032d656befce",
        "Jordan vs Bird - One on One",
        "(Japan)",
        "DMG-JBA",
        1070274381,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "eaa9a7639086b1b7d77ae8a4c3865ba4e9eaca63",
        "Jordan vs Bird - One on One",
        "(USA, Europe)",
        "DMG-JBE",
        3373028438,
        65536,
        "En",
        "USA, Europe"
   , " " },
    {
        "093f5049958a3c24b019299858a6871657edc7b0",
        "Jimmy Connors no Pro Tennis Tour",
        "(Japan)",
        "DMG-JCA",
        409409026,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "816a98f33de163bb72b9f0b3575f6f35d22285da",
        "Jimmy Connors Tennis",
        "(USA, Europe)",
        "DMG-JCE",
        2608775057,
        65536,
        "En",
        "USA, Europe"
   , " " },
    {
        "89d9f76108bffa68f816defea82bb387914c3964",
        "Super James Pond",
        "(Europe)",
        "DMG-JDX",
        1841412908,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "1ed5e04ab6e58b9b7c8f70f3705dd2b4035f2ecf",
        "Jeopardy! - Sports Edition",
        "(USA)",
        "DMG-JEE",
        995242518,
        131072,
        "En",
        "USA"
   , " " },
    {
        "80ddec0550030bddc56b87a9b4abd042bf2e61cb",
        "Janshirou",
        "(Japan)",
        "DMG-JFJ",
        87089612,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "869e4911241b2c8a3eca6359b6ddb8e2f498e07e",
        "Jinsei Game Densetsu",
        "(Japan)",
        "DMG-JGJ",
        1494174460,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "da6dfeca407b82aa0404e1c14307d4959219d349",
        "Jeep Jamboree - Off-Road Adventure",
        "(USA)",
        "DMG-JJE",
        2716297779,
        131072,
        "En",
        "USA"
   , " " },
    {
        "bbd458cdc980b2c2aaa06bbf30bf8d4d11b8e7d8",
        "George Foreman's KO Boxing",
        "(USA, Europe)",
        "DMG-JKE",
        2137146731,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "dfaa5eeecdd9334f02eeadd6479f0ea6753ad349",
        "J.League Fighting Soccer - The King of Ace Strikers",
        "(Japan)",
        "DMG-JLJ",
        392201794,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "c7005678d291f8e543137727c34f91f5edd46a34",
        "Joe & Mac",
        "(USA)",
        "DMG-JME",
        1571319508,
        262144,
        "En",
        "USA"
   , " " },
    {
        "88f04708c2539fb4ca40518c6155dcb105808971",
        "Joe & Mac - Caveman Ninja",
        "(Europe) (En,Fr,De,Es,It,Nl,Sv)",
        "DMG-JMX",
        2614620566,
        262144,
        "En,Fr,De,Es,It,Nl,Sv",
        "Europe"
   , " " },
    {
        "16d925d11d6d7a01841ad915e7212871a3d9d572",
        "Jack Nicklaus Golf",
        "(USA, Europe)",
        "DMG-JNE",
        1699317938,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "54fef766970205d2c355accb97679bf5d325c2ad",
        "Jack Nicklaus Golf",
        "(France)",
        "DMG-JNF",
        927503319,
        131072,
        "Fr",
        "France"
   , " " },
    {
        "fcc58097dd7a23080556450854faf04ede4038e7",
        "Jeopardy!",
        "(USA)",
        "DMG-JPE",
        141712505,
        131072,
        "En",
        "USA"
   , " " },
    {
        "3fc97f5f882b8f981978de3951b5b52fb720efc7",
        "Jurassic Park",
        "(USA)",
        "DMG-JQE",
        3008413407,
        262144,
        "En",
        "USA"
   , " " },
    {
        "630d59dd03063ebf96560e4adcbd91fd47a9565e",
        "Jurassic Park",
        "(Europe) (En,Fr,De,Es,It)",
        "DMG-JQX",
        747295317,
        262144,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "204-10401",
        "Takeda Nobuhiro no Ace Striker",
        "(Japan)",
        "DMG-JRJ",
        2146780895,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "2861fc1debb7191f1f966597d3f823953e37795b",
        "Jetsons, The - Robot Panic",
        "(USA, Europe) (Rev 1)",
        "DMG-JSE",
        3426275085,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "e20196d19e705dadef444ba7a04b14a1d4df7070",
        "Jetsons, The - Robot Panic",
        "(USA, Europe)",
        "DMG-JSE",
        1669777520,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "1aab1081c5648b5120275ea5bb4f46a2155495a9",
        "Simpsons, The - Bart vs. the Juggernauts",
        "(USA, Europe)",
        "DMG-JUE",
        1746521869,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "fae33f449222bff8c53ac5a025644809f980cc02",
        "Goal!",
        "(USA)",
        "DMG-JVE",
        13945586,
        131072,
        "En",
        "USA"
   , " " },
    {
        "49c504e535e94b6cb826fd634639ab741356dcf7",
        "Jaleco J.Cup Soccer",
        "(Japan)",
        "DMG-JVJ",
        2552599127,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "543f533593f4915df3a3a55a812fcdb4cfa85cbb",
        "Goal!",
        "(Europe)",
        "DMG-JVX",
        1396681081,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "d155c3559d82539e915856cf919d4c2a4b93fa7b",
        "Jungle Wars",
        "(Japan)",
        "DMG-JWJ",
        718564039,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "7ee735c73ee940f46c608287e160a9a9501abd96",
        "Downtown Special - Kunio-kun no Jidaigeki Da yo Zenin Shuugou!",
        "(Japan)",
        "DMG-JXJ",
        580817171,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "f84b4dee36c7f3ac677721bc93f87ee7796c9f0d",
        "Tom to Jerry",
        "(Japan)",
        "DMG-JYA",
        3397611117,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "4905169617c01d26a4611f85699800aaba542164",
        "Tom & Jerry",
        "(USA, Europe)",
        "DMG-JYE",
        104257694,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "3e2d093d7e586df0e33ff6ee8efe24bd7453631d",
        "Zen-Nihon Pro Wrestling Jet",
        "(Japan) (SGB)",
        "DMG-JZJ",
        295422677,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "273c2a48415dad2a1a5e88593d60f835032b0c91",
        "Masakari Densetsu - Kintarou RPG Hen",
        "(Japan) (SGB)",
        "DMG-K2J",
        3636115680,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "3a59b282843f87d0c06789c11823ba84e0304c15",
        "Pachi-Slot Kids",
        "(Japan)",
        "DMG-K3J",
        2351015805,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "b8aa5fefad43c626e650b2dd0f9035be72df80eb",
        "Crayon Shin-chan - Ora to Shiro wa Otomodachi Da yo",
        "(Japan)",
        "DMG-K4J",
        647599148,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "2bc2ed5961c4b5f98ac9275160335df7533c5d1d",
        "Kattobi Road",
        "(Japan)",
        "DMG-K5J",
        4022084604,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "9437c9fb24b937ec1a16157333b4bc9c8c42e063",
        "Kachiuma Yosou Keiba Kizoku",
        "(Japan)",
        "DMG-K6J",
        2776505025,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "5c4a5460cc8daf99e23d5652c0faeba32d9426cd",
        "Pachio-kun",
        "(Japan)",
        "DMG-K7J",
        2587766210,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "3a5f80f8a29fa722acc65d603ef3e6b9a403a479",
        "Teenage Mutant Ninja Turtles III - Radical Rescue",
        "(USA)",
        "DMG-K8E",
        1484991420,
        131072,
        "En",
        "USA"
   , " " },
    {
        "a8db556b044eb02c11bb8b44bbecd402078bb315",
        "Teenage Mutant Ninja Turtles 3 - Turtles Kikiippatsu",
        "(Japan)",
        "DMG-K8J",
        1427454901,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "2b457b9b19107b940636fd537f1d477b91654301",
        "Teenage Mutant Hero Turtles III - Radical Rescue",
        "(Europe) (Rev 1)",
        "DMG-K8X",
        1534892628,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "6a32abc879ccf9d4d5e714fc70236d3958538328",
        "Teenage Mutant Hero Turtles III - Radical Rescue",
        "(Europe)",
        "DMG-K8X",
        2670872051,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "5241b7050b3d4c8137b7c5a35c9fe206fe3cef16",
        "Kirby's Pinball Land",
        "(USA, Europe)",
        "DMG-K9E",
        835413286,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "544cd4262952dff627b93adb033cf204a985fb00",
        "Kirby no Pinball",
        "(Japan)",
        "DMG-K9J",
        2336553853,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "f6fa5a82f6e52e8972a5d7fca3a8737fdb1fbb3b",
        "Kid Icarus - Of Myths and Monsters",
        "(USA, Europe)",
        "DMG-KAE",
        201599074,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "e7f4ac6e5aab40d56b49bb318bfd75b306486e8b",
        "Panel no Ninja Kesamaru",
        "(Japan)",
        "DMG-KBJ",
        315269578,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "36a158c6b7a8a14f8ea663607337a78b77173709",
        "Kingdom Crusade",
        "(USA)",
        "DMG-KCE",
        2449605340,
        131072,
        "En",
        "USA"
   , " " },
    {
        "727f208a7f3b6f991451d4116354d42344269c1a",
        "Command Master",
        "(Japan)",
        "CGB-KCEJ",
        3507181125,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "e9e55f62d6acef5397199e15a3aaa251e3a73df2",
        "Legend of Prince Valiant, The",
        "(Europe) (En,Fr,De)",
        "DMG-KCX",
        396118350,
        131072,
        "En,Fr,De",
        "Europe"
   , " " },
    {
        "a6c074f754dcaef52b161d68a366c58fe2f45dfa",
        "Skate or Die - Tour de Thrash",
        "(USA)",
        "DMG-KDE",
        45579273,
        131072,
        "En",
        "USA"
   , " " },
    {
        "ac2e075a24b0a64fcb20f94d658244baf9fa7540",
        "Pro Soccer",
        "(Japan)",
        "DMG-KFJ",
        2800991970,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "5ed78cb7416c6cff7fdd871a049872bae7524477",
        "Super Kick Off",
        "(Europe) (En,Fr,De,It,Nl)",
        "DMG-KFX",
        1260256829,
        131072,
        "En,Fr,De,It,Nl",
        "Europe"
   , " " },
    {
        "abfd29389296c16a9e12e9b2ab5d51730edda937",
        "Konamic Golf",
        "(Japan)",
        "DMG-KGA",
        3245166002,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "c052364c8ec6a1a11e4c87b2e3219601844557d5",
        "Ultra Golf",
        "(USA)",
        "DMG-KGE",
        316578732,
        131072,
        "En",
        "USA"
   , " " },
    {
        "bc99fc3645b71e06454bed05faccb494a7e9b7ec",
        "Konami Golf",
        "(Europe)",
        "DMG-KGX",
        266117041,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "03c2d56b4a1ab84c53d6e643c153bb4a4f878e6e",
        "Track & Field",
        "(USA, Europe)",
        "DMG-KHE",
        3236903408,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "f1e112f02a738300e42d5c493c00d9cf58b3ccf2",
        "Konamic Sports in Barcelona",
        "(Japan)",
        "DMG-KHJ",
        1461839830,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "91a3cc61d89733117bda1fcbea52a6d21b49f79c",
        "Koi wa Kakehiki",
        "(Japan)",
        "DMG-KIJ",
        4091668050,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "77fc86b7b2ab801815faaa2a06da5089cb3dcb24",
        "Kingyo Chuuihou! 2 - Gyopi-chan o Sagase!",
        "(Japan)",
        "DMG-KJJ",
        4090277315,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "7fa385ac2702da4cc7258027f35131f3e454d2f1",
        "Koro Dice",
        "(Japan)",
        "DMG-KKA",
        962628657,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "51-11057",
        "Korokoro Kirby",
        "(Japan)",
        "CGB-KKKJ",
        3237662269,
        1048576,
        "Ja",
        "Japan", 
        "/kirby_tilt64.sprite" },
    {
        "41042111bb29ed4f10eabe387e7cfb3a86cb6081",
        "Klax",
        "(Japan) (Hudson Soft)",
        "DMG-KLA",
        3029686409,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "33114785d4490aa4de99b2c5c998e8ef085f794c",
        "Serpent",
        "(USA)",
        "DMG-KME",
        1960077015,
        32768,
        "En",
        "USA"
   , " " },
    {
        "740142714470cbe434bfa1366abd583645a39d7e",
        "Kakomunja",
        "(Japan)",
        "DMG-KMJ",
        3203530283,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "5afb041e845306f2748b14363c4ae00737fb7063",
        "Cave Noire",
        "(Japan)",
        "DMG-KNJ",
        1143302703,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "d8e3d00b7db645a59cc6720a90478c85e81bfbee",
        "Masakari Densetsu - Kintarou Action Hen",
        "(Japan)",
        "DMG-KOJ",
        2092371261,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "8cb7212727cf613dc7d03ce0e663b4d4be480e2b",
        "Kitchen Panic",
        "(Japan)",
        "DMG-KPJ",
        2425960395,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "3745ab7fbe20f98208e25f47d0ef6480bb24b449",
        "Magnetic Soccer",
        "(Europe)",
        "DMG-KQX",
        2620492900,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "8b6657dc9bcbbbdd77bde1d04f123e28d9330f90",
        "Master Karateka",
        "(Japan)",
        "DMG-KRA",
        1276097342,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "6fe4351ce16dd39742f68b9dc089d08f2b95590f",
        "Rubble Saver",
        "(Japan)",
        "DMG-KSA",
        3903759442,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "72-4882",
        "Adventures of Star Saver, The",
        "(USA, Europe)",
        "DMG-KSE",
        1564873588,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "4e964f2a0f6cf5fb7835c0e86a023dda3eea2c58",
        "Kingyo Chuuihou! - Wapiko no Wakuwaku Stamp Rally!",
        "(Japan)",
        "DMG-KTJ",
        3868880650,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "9c3e7f8bc111cad3093e666f301cde6cbcbf116d",
        "Kirby - Tilt 'n' Tumble",
        "(USA)",
        "CGB-KTNE",
        3846286577,
        1048576,
        "En",
        "USA", 
        "/kirby_tilt64.sprite" },
    {
        "89dda3c2ebdc693b4f98854726b6eaa97ef448d5",
        "Nekketsu Kouha Kunio-kun - Bangai Rantou Hen",
        "(Japan)",
        "DMG-KUJ",
        632429260,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "6e9ed1aded7e0019eb8b739dbdd2681fca890028",
        "Rubble Saver II",
        "(Japan)",
        "DMG-KVA",
        2462670637,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "5254307de9044a07ffb5a966bb824e91fb9ec5da",
        "Max",
        "(Europe)",
        "DMG-KVX",
        454459136,
        65536,
        "En",
        "Europe"
   , " " },
    {
        "c57fb1b20704b95e92ce6ef3c371cf7790b165cf",
        "Kinnikuman - The Dream Match",
        "(Japan)",
        "DMG-KWJ",
        1220726072,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "77b0e9d49843575e016d00418a67516abbb7cca0",
        "Klax",
        "(USA) (Mindscape)",
        "DMG-KXE",
        1919289204,
        65536,
        "En",
        "USA"
   , " " },
    {
        "78a4fc988d6ec1aa883caaa23f0fdfb2a2206506",
        "Kirby's Dream Land",
        "(USA, Europe)",
        "DMG-KYE",
        1089623872,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "0febbecb8775fff07fd481c5f4c9ed72f22e23b7",
        "Hoshi no Kirby",
        "(Japan) (Rev 1)",
        "DMG-KYJ",
        70528131,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "5d8b29da4c318362f551a6c44d8469cbd0e3d239",
        "Hoshi no Kirby",
        "(Japan)",
        "DMG-KYJ",
        553869205,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "fe3a8f58a1afcab41ca19224d4d6067a3a75809e",
        "Little Master 2 - Raikou no Kishi",
        "(Japan)",
        "DMG-L2J",
        410904568,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "0d4ffd081c80a549336295d77e02074a3b12d0eb",
        "Alien 3",
        "(Japan)",
        "DMG-L3A",
        3851438514,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "0a3fc6fb5f5dd7789872554b9cb74a0b8ed3b20b",
        "Alien 3",
        "(USA, Europe)",
        "DMG-L3E",
        4026242571,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "7fea218b75a9d7b80c1b96fe824f39db9366ccff",
        "Mickey Mouse IV - Mahou no Labyrinth",
        "(Japan)",
        "DMG-L4A",
        221295521,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "fe46751b649e4713775673747e60c8083020366e",
        "Real Ghostbusters, The",
        "(USA)",
        "DMG-L4E",
        3780717443,
        131072,
        "En",
        "USA"
   , " " },
    {
        "47c6d22283d3d11dc4870dc1f11a2d7f45dec172",
        "Garfield Labyrinth",
        "(Europe)",
        "DMG-L4X",
        1371038317,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "08034435e2f6010e854dc98b909b791cae489331",
        "Virtual Wars",
        "(Japan)",
        "DMG-L5J",
        571194550,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "36d1adf9bb60ae3f18a7b5efe8ee264c55782f43",
        "Lawnmower Man, The",
        "(Europe)",
        "DMG-L5X",
        323981783,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "3e19e92b1c1f1344da767883fe84ff3079268900",
        "Super Mario Land 2 - 6-tsu no Kinka",
        "(Japan)",
        "DMG-L6J",
        2803227381,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "90553b98524885b5e6842b3cb5562ab6928f8bb3",
        "Super Mario Land 2 - 6-tsu no Kinka",
        "(Japan) (Rev 1)",
        "DMG-L6J",
        3212000784,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "f7bad585030936299ca74f3d1ed03491d1db529e",
        "Super Mario Land 2 - 6-tsu no Kinka",
        "(Japan) (Rev 2)",
        "DMG-L6J",
        702583066,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "7e0911c74f725e26aab2b4a8efb4747951825f68",
        "Lamborghini American Challenge",
        "(USA, Europe)",
        "DMG-L7E",
        1734218505,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "ecaa87d8713e6f0028aac94830552cc758dc2776",
        "Lemmings",
        "(Japan)",
        "DMG-L8A",
        2103230522,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "ed0369b2286e4fbcfe9debda7ddeec2674837028",
        "Lemmings",
        "(USA)",
        "DMG-L8E",
        4073832861,
        131072,
        "En",
        "USA"
   , " " },
    {
        "c5442c9ea8cfaa4e8f74dc6cfc28de3f7f4e9464",
        "Lemmings",
        "(Europe)",
        "DMG-L8X",
        591821365,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "ecc24a563cfbdf3ccf94141e6099eb8e698aae1b",
        "Lemmings",
        "(Europe) (Rev 1)",
        "DMG-L8X",
        1443721707,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "624d97461a1aa7f7a847de00a91aa0d1f5bc9f6b",
        "Little Master - Raikuban no Densetsu",
        "(Japan)",
        "DMG-LAJ",
        835556976,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "dd9575d82c7233bc98713ff5020629578eb78373",
        "Lock n' Chase",
        "(World)",
        "DMG-LCA",
        3669564538,
        65536,
        "En",
        "World"
   , " " },
    {
        "d0492517cbe72ac8fc96ac44e48b101bc4f72feb",
        "Little Mermaid, The",
        "(USA)",
        "DMG-LDE",
        3620018149,
        131072,
        "En",
        "USA"
   , " " },
    {
        "fc2840d2335cf62c95d3aaa5f90cf8528c9a8bfa",
        "Little Mermaid, The",
        "(Europe)",
        "DMG-LDX",
        14611322,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "1c5cff0bece7163c6fcc8d13484b6c11fc2b2174",
        "Kick Boxing, The",
        "(Japan)",
        "DMG-LEA",
        2153133132,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "8c354812a717a85f5113110d18ce6197808ee735",
        "Best of the Best - Championship Karate",
        "(USA)",
        "DMG-LEE",
        2986508518,
        262144,
        "En",
        "USA"
   , " " },
    {
        "a43e5b71805913639adc6d1064e38fd1800d0974",
        "Best of the Best - Championship Karate",
        "(Europe)",
        "DMG-LEX",
        3586382365,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "86ffdee09035de660291358a2126303d685238d3",
        "Legend - Ashita e no Tsubasa",
        "(Japan)",
        "DMG-LGJ",
        4230015984,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "4a2e974fd93ff87077af8301b630282f457c205a",
        "SD Gundam Gaiden - Lacroan' Heroes",
        "(Japan)",
        "DMG-LHJ",
        3386620432,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "4ffdb5eee2bbe767739e93374f1b4fb04296a4a1",
        "Lunar Lander",
        "(Japan)",
        "DMG-LLA",
        51191892,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "b0c1a643336a8f8925984dfaeaeb5cd247a71281",
        "Spanky's Quest",
        "(USA)",
        "DMG-LME",
        1860684409,
        65536,
        "En",
        "USA"
   , " " },
    {
        "7999395306b9932e7be2b76223b63ff84a1fe60f",
        "Lucky Monkey",
        "(Japan)",
        "DMG-LMJ",
        2720222897,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "e504a5bc141c326c215cc9b8b5c7bd6905b9ca8a",
        "Spanky's Quest",
        "(Europe)",
        "DMG-LMX",
        3924787510,
        65536,
        "En",
        "Europe"
   , " " },
    {
        "cdce955eb072669de19a65b879a6b7f5c4c7374e",
        "Looney Tunes",
        "(USA, Europe)",
        "DMG-LNE",
        2791483631,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "46b7785f1a49c18a7b2ca72e4f14e43e0e918a3f",
        "Looney Tunes - Bugs Bunny to Yukai na Nakama-tachi",
        "(Japan)",
        "DMG-LNJ",
        1088716195,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "489a21756203c1dfca4545ea7c539bce79e4ad72",
        "Lolo no Daibouken",
        "(Japan)",
        "DMG-LOJ",
        3591919937,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "227-1918",
        "Adventures of Lolo",
        "(Europe) (SGB)",
        "DMG-LOX",
        393031403,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "f042e9bbb3cea24391ed725da9c4726b372874bd",
        "Loopz",
        "(World)",
        "DMG-LPA",
        1394228758,
        32768,
        "En",
        "World"
   , " " },
    {
        "15483a42fe8da2d8aab0e17bc3d274df660b4fec",
        "Spider-Man 3 - Invasion of the Spider-Slayers",
        "(USA, Europe)",
        "DMG-LQE",
        3417502252,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "61147dc7ff3a58d26275ae57ca621233cd3f11b6",
        "Last Action Hero",
        "(USA, Europe)",
        "DMG-LRE",
        273259254,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "258aaae5d521a032c3125325e58842d2d4dc1291",
        "TaleSpin",
        "(USA)",
        "DMG-LSE",
        3728227955,
        131072,
        "En",
        "USA"
   , " " },
    {
        "35690544fca168dfbac3bb1e2937759b409fbf2d",
        "TaleSpin",
        "(Europe)",
        "DMG-LSX",
        3533560977,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "9aa2d862374c74c507960694937df3b27f2041ec",
        "Lethal Weapon",
        "(USA, Europe)",
        "DMG-LTE",
        529342588,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "840749301adfbe0d18d581fd6b648647de3c12d0",
        "Lucle",
        "(Japan, Europe) (En)",
        "DMG-LUA",
        2748696791,
        524288,
        "En",
        "Japan, Europe"
   , " " },
    {
        "ab5503da976fc03edfae1eec83b0905568467324",
        "WWF Superstars",
        "(USA, Europe)",
        "DMG-LWE",
        2914799114,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "9b8087eb9da6284c2fe650b0e25a655beed69ffe",
        "WWF Superstars",
        "(Japan)",
        "DMG-LWJ",
        4069415585,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "7ef465a7e68893b7a88fd74cbee57cbd34b73674",
        "Bubble Bobble Junior",
        "(Japan)",
        "DMG-LXA",
        971137465,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "7fcd952eec0c0f7fa69331be535323448182cfbb",
        "Bubble Bobble Part 2",
        "(USA, Europe)",
        "DMG-LXE",
        1090967425,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "3e4699acca464465866a1879e9fbd0d019bb8402",
        "Monopoly",
        "(USA)",
        "DMG-LYE",
        4254134486,
        131072,
        "En",
        "USA"
   , " " },
    {
        "9283608d1ef305d8cc626a32432e881fa1cb3223",
        "Monopoly",
        "(Japan)",
        "DMG-LYJ",
        3635153077,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "967541184d30a46a7bcc40a1a12ce2e96f020467",
        "Lingo",
        "(Europe) (En,Fr,De,Nl)",
        "DMG-LZX",
        250897277,
        131072,
        "En,Fr,De,Nl",
        "Europe"
   , " " },
    {
        "b00feda6b9857ebf8a0ffc7f68105f5e81e5020a",
        "Mickey Mouse II",
        "(Japan)",
        "DMG-M2A",
        3756952524,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "3f835ef2c824ac7eae5253952d5680814c51c9c5",
        "Mickey Mouse",
        "(Europe)",
        "DMG-M2X",
        4233158375,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "21327feae817cc96eee7d09bad94134b8555d488",
        "Chibi Maruko-chan 3 - Mezase! Game Taishou no Maki",
        "(Japan)",
        "DMG-M3J",
        1156133832,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "174b7f63c47c87283b3ecda491d72faf4f2df905",
        "Mr. Do!",
        "(USA)",
        "DMG-M4E",
        2702323648,
        65536,
        "En",
        "USA"
   , " " },
    {
        "2c08926994055d9d6a637e20f60e234d4d68bd17",
        "Mr. Do!",
        "(Europe)",
        "DMG-M4X",
        3481578615,
        65536,
        "En",
        "Europe"
   , " " },
    {
        "a8d6adcc7b4a124ad3e17b471695c96e9d094871",
        "Kikou Keisatsu Metal Jack",
        "(Japan)",
        "DMG-M5J",
        4090256994,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "556a1c2581155d4075a805e3ec1f74152b6adca2",
        "Monster Maker 2 - Uru no Hiken",
        "(Japan)",
        "DMG-M6J",
        1726417148,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "c86b991c0f4a920ad5e686c31f0b7c0d5fe1590b",
        "Megami Tensei Gaiden - Last Bible",
        "(Japan)",
        "DMG-M7J",
        4271756480,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "beaa7440ba4872aadd5d0daa6a2004f24060d52e",
        "Milon's Secret Castle",
        "(USA, Europe)",
        "DMG-M8E",
        1656016012,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "9be921c4347f826fb9df734ce78ee1d250884de4",
        "Milon no Meikyuu Kumikyoku",
        "(Japan)",
        "DMG-M8J",
        4052273484,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "51631dca3adbb0e83d59bcab3a2fb3c4bb8b497b",
        "Megami Tensei Gaiden - Last Bible II",
        "(Japan)",
        "DMG-M9J",
        4175763929,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "025ed7c18b546208d995dc802e2f42a7d10d784d",
        "Shisenshou - Match-Mania",
        "(Japan)",
        "DMG-MAA",
        215845636,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "3954be1645b0cd3e1394b0f20c86a22067505aef",
        "Marble Madness",
        "(USA, Europe)",
        "DMG-MBE",
        3111280230,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "f86df05274fe81387900191f3d151b68636fd06f",
        "Mickey's Dangerous Chase",
        "(USA)",
        "DMG-MCE",
        2072128911,
        131072,
        "En",
        "USA"
   , " " },
    {
        "e2b65e38808cb271cf296214b21fc67a5b378714",
        "Mickey's Chase",
        "(Japan)",
        "DMG-MCJ",
        179605704,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "6cd092b5548019b8e56e48f6b412ecce8261f02c",
        "Mickey's Dangerous Chase",
        "(Europe)",
        "DMG-MCX",
        3878532962,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "84898ed91480b386252be1940f86b88ea81db0df",
        "Super Momotarou Dentetsu",
        "(Japan)",
        "DMG-MDJ",
        1286002970,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "151-22559",
        "Metroid II - Return of Samus",
        "(World)",
        "DMG-MEA",
        3739243376,
        262144,
        "En",
        "World"
   , "/metroid_264.sprite" },
    {
        "0a6edb4878a6ed16ec3d822abb8080be6ae0de7c",
        "Magical Taruruuto-kun 2 - Raibaa Zone Panic!!",
        "(Japan)",
        "DMG-MFJ",
        179130903,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "32c9b9b69c34335f911e3f881a8a37bf8110d5f8",
        "Magical Taruruuto-kun",
        "(Japan)",
        "DMG-MGJ",
        2622444785,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "e0b088a3b77454038828ea4b07351e3d738d5c76",
        "Mouse Trap Hotel",
        "(USA)",
        "DMG-MHE",
        1860110309,
        65536,
        "En",
        "USA"
   , " " },
    {
        "c4dd4ffb1840d40be598c30ec41f93dbfd997295",
        "Momotarou Densetsu Gaiden",
        "(Japan)",
        "DMG-MIJ",
        3126202863,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "25867b212ab4391a05b2f9e50ee68b193c584904",
        "Yakuman",
        "(Japan) (Rev 1)",
        "DMG-MJJ",
        2429247681,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "fbcc18c6f73cf70391a2a82db2b76b5c29d13c2a",
        "Yakuman",
        "(Japan)",
        "DMG-MJJ",
        825551456,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "d54b99fdee57e99cb7cd2b519d83725319fb4fdd",
        "Mikeneko Holmes no Kishidou",
        "(Japan)",
        "DMG-MKJ",
        1941441302,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "dcc1394fe236c9f73edaa534084482410ff774c9",
        "Super Mario Land",
        "(World)",
        "DMG-MLA",
        2423744577,
        65536,
        "En",
        "World"
   , " " },
    {
        "de9e8c845079491f083ca8ce841314fe295a4d83",
        "Super Mario Land",
        "(World) (Rev 1)",
        "DMG-MLA",
        740813936,
        65536,
        "En",
        "World"
   , " " },
    {
        "479badc771da24e821e276f8137793f8825d63ca",
        "Mickey Mouse",
        "(Japan)",
        "DMG-MMA",
        1286036813,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "555002099f43e6d4772011f4e5f6376ff9296966",
        "GB Memory Multi Menu",
        "(Japan) (SGB Enhanced, GB Comp.) (NP)",
        "DMG-MMSA",
        3967958209,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "19b46f5e80b0165cb4a52a952cdcd78ab207501c",
        "Kidou Keisatsu Patlabor - Nerawareta Machi 1990",
        "(Japan)",
        "DMG-MOJ",
        3354914988,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "054490e7e8a47fe7d359d0cb102088599dfbea0c",
        "Paperboy",
        "(USA, Europe)",
        "DMG-MPE",
        1188967098,
        65536,
        "En",
        "USA, Europe"
   , " " },
    {
        "2d65e4821c7fdf4b04d0a38f218269a9f15cb9d5",
        "Super Mario Land 2 - 6 Golden Coins",
        "(USA, Europe) (Rev 1)",
        "DMG-MQE",
        3875047141,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "17-64238",
        "Super Mario Land 2 - 6 Golden Coins",
        "(USA, Europe)",
        "DMG-MQE",
        3589022948,
        524288,
        "En",
        "USA, Europe"
   , "/mario_land_264.sprite" },
    {
        "ca47959a6612d96503c0b1bc014370a07bc2788a",
        "Super Mario Land 2 - 6 Golden Coins",
        "(USA, Europe) (Rev 2)",
        "DMG-MQE",
        1666879762,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "048b62ff807531bd026fb9edf46bfde9246a13e9",
        "Maerchen Club",
        "(Japan)",
        "DMG-MQJ",
        3455936010,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "d5d9db1c22e1d3e8e655a02a26468d71b5fbf47d",
        "Monster Maker",
        "(Japan)",
        "DMG-MRJ",
        1918822508,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "dcce7ec5c8f1e0ba0ad04e442cf1fbbfb968c4d6",
        "Monster Truck",
        "(Japan)",
        "DMG-MTE",
        2885268759,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "5aded45734540e651d5f38c0328dd6bcd49fc57b",
        "Karakuri Kengou Den Musashi Lord",
        "(Japan)",
        "DMG-MUJ",
        28238411,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "9c9a5d0f85e58e5863df66712cfbee61ff3a789b",
        "Miner 2049er Starring Bounty Bob",
        "(USA)",
        "DMG-MVE",
        443274615,
        65536,
        "En",
        "USA"
   , " " },
    {
        "2b2fb1ac2e4baea908c511aa56d747c6f4363c3f",
        "Missile Command",
        "(USA, Europe)",
        "DMG-MWE",
        980210904,
        32768,
        "En",
        "USA, Europe"
   , " " },
    {
        "f0ac85b6f5e59bd1bd48cba8e53ac223830700fe",
        "Motocross Maniacs",
        "(Japan)",
        "DMG-MXA",
        3663745987,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "c7212f52d2dd65c86730ae347aa869a9bea0cdc6",
        "Motocross Maniacs",
        "(USA)",
        "DMG-MXE",
        831372769,
        32768,
        "En",
        "USA"
   , " " },
    {
        "411393d34a83087180ab2f10e9de5c99619fe58e",
        "Motocross Maniacs",
        "(Europe)",
        "DMG-MXX",
        3798562424,
        32768,
        "En",
        "Europe"
   , " " },
    {
        "95204056d9063ee23e221a2c9a7f9f34b0c77235",
        "Motocross Maniacs",
        "(Europe) (Rev 1)",
        "DMG-MXX",
        4167560763,
        32768,
        "En",
        "Europe"
   , " " },
    {
        "a82978f162d162dba2f0b12f7d386084dab82fb4",
        "Mysterium",
        "(USA)",
        "DMG-MYE",
        3944898198,
        131072,
        "En",
        "USA"
   , " " },
    {
        "de6d8088d7ddff5b0f0c13a9d4cd3860424ab586",
        "Mysterium",
        "(Japan)",
        "DMG-MYJ",
        874573290,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "fca8761acee03f72223bf1261fdf9f92022eec0c",
        "Megalit",
        "(USA, Europe)",
        "DMG-MZE",
        4078286914,
        65536,
        "En",
        "USA, Europe"
   , " " },
    {
        "e9064950a04aa84ea631e538c176679c32a470ae",
        "Megalit",
        "(Japan)",
        "DMG-MZJ",
        3345265138,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "9295829e3797382e82c14d7b5a3c8428b9a19895",
        "Famista 3",
        "(Japan)",
        "DMG-N3J",
        3183791299,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "852ccebff365782bb5c53a6d9d22cc4de412a30f",
        "Ms. Pac-Man",
        "(USA)",
        "DMG-N4E",
        240890308,
        65536,
        "En",
        "USA"
   , " " },
    {
        "4a9635efc3c623134c57d79ae41cbc5b7d3d80fe",
        "Ms. Pac-Man",
        "(Europe)",
        "DMG-N4X",
        3785320407,
        65536,
        "En",
        "Europe"
   , " " },
    {
        "70cf74f0278fde5a5c79229ea5560f5e92f18482",
        "New Chessmaster, The",
        "(USA, Europe)",
        "DMG-N5E",
        3355427331,
        65536,
        "En",
        "USA, Europe"
   , " " },
    {
        "94df37867afa71a12aef85811c299803879b3087",
        "Super Battletank",
        "(USA)",
        "DMG-N6E",
        3660048280,
        131072,
        "En",
        "USA"
   , " " },
    {
        "8b6975c20b0c96f33941e1b3c8b182e049457c40",
        "Super Battletank",
        "(Europe)",
        "DMG-N6X",
        832710637,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "3c04da9085e13ca20bba924c083ccccc9c3d2421",
        "Pokonyan! - Yume no Daibouken",
        "(Japan) (SGB)",
        "DMG-N7J",
        1640186418,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "5bb29a3a29441e95bb7fd40ebaef6d5aec0648ce",
        "Total Carnage",
        "(USA, Europe)",
        "DMG-N8E",
        922784383,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "3cb71cf4a0da1fbbe5437a616edd27c383749db2",
        "Kaisen Game - Navyblue 90",
        "(Japan)",
        "DMG-N9J",
        2365502178,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "751388df810f15364c914a18366a6170a0ffc60c",
        "Kaisen Game - Navyblue",
        "(Japan)",
        "DMG-NBA",
        2950218876,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "aaaccaf45f979ee7b2e7b76e7de1fa478430ba78",
        "Battleship",
        "(USA, Europe)",
        "DMG-NBE",
        4277546405,
        65536,
        "En",
        "USA, Europe"
   , " " },
    {
        "0cb3887af67e41a8e7da0943fb3204f54b249d49",
        "Nintendo World Cup",
        "(USA, Europe)",
        "DMG-NCE",
        2529316371,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "420a552b0a2798691730f511d653a5dc08dfaad1",
        "Nekketsu Koukou Soccer-bu - World Cup Hen",
        "(Japan)",
        "DMG-NCJ",
        1354935713,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "dc5fc5bb7b181de83da28fedb4af3341aee9b1ca",
        "Nekketsu Koukou Dodgeball-bu - Kyouteki! Dodge Soldier no Maki",
        "(Japan)",
        "DMG-NDJ",
        2125424410,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "16ad2bf211a966823431c885566465adc5dbdd20",
        "Nemesis II",
        "(Japan)",
        "DMG-NEA",
        2891646509,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "188eb2048a0757e7a9f4b2edbbc335600341c1e1",
        "Gradius - The Interstellar Assault",
        "(USA)",
        "DMG-NEE",
        2432204119,
        262144,
        "En",
        "USA"
   , " " },
    {
        "088d8556efb5244e096e795ae3812b9d94efdeef",
        "Nemesis II - The Return of the Hero",
        "(Europe)",
        "DMG-NEX",
        3286641205,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "9faa7a7d4bf1bbb0a0bc7928a9d5cd72066f6f61",
        "Ninja Gaiden Shadow",
        "(USA)",
        "DMG-NGE",
        3547601466,
        131072,
        "En",
        "USA"
   , " " },
    {
        "537b5f36f261a868524a7c0b43214258258b4fde",
        "Ninja Ryuuken Den - Matenrou Kessen",
        "(Japan)",
        "DMG-NGJ",
        3535680407,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "d409dc14d54c81382c7957836971cc26b7d5b0eb",
        "Shadow Warriors",
        "(Europe)",
        "DMG-NGX",
        3166801444,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "395a57944ffd6632f595f082269cf47466efa0b0",
        "Namco Classic",
        "(Japan)",
        "DMG-NHJ",
        3962722227,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "e192479bb822b813f1d16b5421c53aff40d55966",
        "Welcome Nakayoshi Park",
        "(Japan)",
        "DMG-NIJ",
        4142054062,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "7c2beb10833b9c41a918901990ce63ba761fccad",
        "Nekojara Monogatari",
        "(Japan)",
        "DMG-NJJ",
        1030566043,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "7d11ee9d72ca909b9bd2caae836635f4bb64d7e8",
        "Sneaky Snakes",
        "(USA, Europe)",
        "DMG-NKE",
        2079591805,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "2c7cb3ea76fc2d66f44db2ee86b7e25ff3a8bcf2",
        "InfoGenius Productivity Pak - Berlitz Spanish Translator",
        "(USA, Europe)",
        "DMG-NLE",
        93253339,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "1957252d4bb36308e12979c2cec56eca6f1a9b91",
        "Nemesis",
        "(USA)",
        "DMG-NMA",
        1360062833,
        131072,
        "En",
        "USA"
   , " " },
    {
        "79074f3dd25a6f87b25d3d473e8292c6191432ed",
        "Nemesis",
        "(Japan)",
        "DMG-NMA",
        3006847719,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "d09dd4eb8e8237b0c76797ccc5a466e9c22aa9f5",
        "Nemesis",
        "(Europe)",
        "DMG-NMX",
        429067941,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "c99c0dc05b4e1a2250411fa2618b140ee435372e",
        "Nanonote",
        "(Japan)",
        "DMG-NNJ",
        1043379773,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "31bc889508555ce0061b30a4a31825e5fd1fe24d",
        "Pac-Panic",
        "(Japan) (SGB)",
        "DMG-NOA",
        2867582217,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "a9ac4cb5ec942a53197c2ee2adbefd107eb4c3ad",
        "Pac-Attack",
        "(USA) (SGB)",
        "DMG-NOE",
        2497752988,
        131072,
        "En",
        "USA"
   , " " },
    {
        "7ab1980f3e19f5c8cfa4b0ee712c133b02acd735",
        "Pac-Panic",
        "(Europe) (SGB)",
        "DMG-NOX",
        3836710773,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "fa3942cee7c86cbd167e7f6dde68d25649e8010f",
        "Prince of Persia",
        "(USA)",
        "DMG-NPE",
        735679985,
        131072,
        "En",
        "USA"
   , " " },
    {
        "05a1a59ae47a50506b177334be615e44ada3f816",
        "Prince of Persia",
        "(Japan)",
        "DMG-NPJ",
        488035977,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "91559d9034ff8e9e5a4ecaef111e8678c4be0764",
        "Prince of Persia",
        "(Europe) (En,Fr,De,Es,It)",
        "DMG-NPX",
        2999710223,
        131072,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "b8f63087c21399f90287c87b4918406ce1e39756",
        "Bikkuri Nekketsu Shin Kiroku! - Dokodemo Kin Medal",
        "(Japan)",
        "DMG-NQJ",
        2261851408,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "6cefd8c183a7e957e43fc5731662afe24abb0869",
        "Nangoku Shounen Papuwa-kun - Ganmadan no Yabou",
        "(Japan)",
        "DMG-NRJ",
        2039427658,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "8aa1a656c908595bb6af03d3ccc654f10e41d2d1",
        "Punisher, The - The Ultimate Payback",
        "(USA)",
        "DMG-NSE",
        1494643006,
        131072,
        "En",
        "USA"
   , " " },
    {
        "ba6d1d3a1b3b20a2776507c517e83f6db7af260a",
        "Teenage Mutant Ninja Turtles - Fall of the Foot Clan",
        "(USA)",
        "DMG-NTE",
        1516864707,
        131072,
        "En",
        "USA"
   , " " },
    {
        "bdbeacff1a4dd8d5241da970cc96afee947b9d75",
        "Teenage Mutant Ninja Turtles",
        "(Japan)",
        "DMG-NTJ",
        1946649142,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "6597a2398d4d59e4e2efacfd6313753b35fd0388",
        "Teenage Mutant Hero Turtles - Fall of the Foot Clan",
        "(Europe)",
        "DMG-NTX",
        3985273851,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "964d462eead6752d01d7dfd6090b3b61df8e04e3",
        "Star Trek - The Next Generation",
        "(Germany)",
        "DMG-NUD",
        311826099,
        131072,
        "De",
        "Germany"
   , " " },
    {
        "4d4fe8b100869b72d26ee935e915a22106fa14a8",
        "Star Trek - The Next Generation",
        "(USA, Europe)",
        "DMG-NUE",
        978998987,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "ce882119bf5a92ba160e268ff2403891476dd939",
        "Star Trek - The Next Generation",
        "(Spain)",
        "DMG-NUS",
        1340004804,
        131072,
        "Es",
        "Spain"
   , " " },
    {
        "dc3011427f164b442c1538cba9c48e08189bd49a",
        "Navy SEALs",
        "(USA, Europe)",
        "DMG-NVE",
        695003851,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "be27420aece75004b093297418b8419d9e93422b",
        "Noobow",
        "(Japan)",
        "DMG-NWJ",
        2344405179,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "ddeacc3e897d3ab88b10973115d39196995bfb31",
        "Battle Space",
        "(Japan)",
        "DMG-NXJ",
        2821258575,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "d6bd7b15f45e111389e787604dfbef906ad0a24b",
        "Nobunaga's Ambition",
        "(USA)",
        "DMG-NYE",
        1518612488,
        131072,
        "En",
        "USA"
   , " " },
    {
        "a8835fe025dea73186e08e6ae040f831820c339c",
        "Nobunaga no Yabou - Game Boy Ban",
        "(Japan)",
        "DMG-NYJ",
        2281904234,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "37bf77f5d9bed7ff693a9927299c20b6e6435ed4",
        "Tekichuu Rush",
        "(Japan)",
        "DMG-NZJ",
        2073049711,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "028ca179d36c93f742f5c33fc91d7cf2e58551a6",
        "Oni II - Oni Densetsu",
        "(Japan)",
        "DMG-O2J",
        797531637,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "28f537e0ac3bb603ec2e17778080b4143cefa388",
        "Oni III - Kuro no Hakaishin",
        "(Japan)",
        "DMG-O3J",
        1825976283,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "f8f1fad71184119cfc57cdda424a58008d6272ad",
        "Oni IV - Kijin no Ketsuzoku",
        "(Japan)",
        "DMG-O4J",
        3204403031,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "232164ec7b7c8579042c564879871b5cce99cc97",
        "Raging Fighter",
        "(USA, Europe)",
        "DMG-OBE",
        519290587,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "cc4ec7c3e291c773819ea23eb2c1fe9b640253fe",
        "Outburst",
        "(Japan)",
        "DMG-OBJ",
        2331821232,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "59147afd6ffc1aa1c1a7c9dd9b2f991ace26695d",
        "GB Basketball",
        "(Japan)",
        "DMG-OFJ",
        3652341025,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "3e701b20f7f44242fb6769d8ed0dc953e0f82457",
        "Tip Off",
        "(Europe)",
        "DMG-OFX",
        3941934060,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "d7d9dfdab7e874983316141623a60bc050e38e2c",
        "Onigashima Pachinkoten",
        "(Japan)",
        "DMG-OGJ",
        4115581163,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "6abddf16dcb7347ed4e1435b0c09347168747a42",
        "Pachi-Slot Kids 2",
        "(Japan)",
        "DMG-OHJ",
        3735872560,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "baa8f96651108f7c35b09146b0db610125f152da",
        "Maru's Mission",
        "(USA)",
        "DMG-OJE",
        1850678963,
        131072,
        "En",
        "USA"
   , " " },
    {
        "94d27aa6434ab760612d4979078664263e57b8ed",
        "Oira Jajamaru! Sekai Daibouken",
        "(Japan)",
        "DMG-OJJ",
        755552895,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "155705af228d489d5aee2d8b18c96c1710f46d51",
        "Kaeru no Tame ni Kane wa Naru",
        "(Japan)",
        "DMG-OKJ",
        3247232378,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "c1f1fd0dd272ee17b9df140db3556b1c61048a68",
        "Battle of Olympus, The",
        "(Europe) (En,Fr,De,Es,It)",
        "DMG-OLX",
        3393523347,
        262144,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "108b9c6af4352c88eceb5eded0f67a3300960295",
        "Sakigake!! Otoko Juku - Meioutou Kessen",
        "(Japan)",
        "DMG-OMJ",
        789544803,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "5b4f346df5332881ea820d3037e3a985990db8f2",
        "Kinin Koumaroku Oni",
        "(Japan)",
        "DMG-ONJ",
        3086255689,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "893a8f6810a0e0828b414939168560ddc8dd7eeb",
        "Kinin Koumaroku Oni",
        "(Japan) (Rev 1)",
        "DMG-ONJ",
        2868262508,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "5199b6ba46c902ec3d5ad389313ff060a22894b6",
        "Cool Ball",
        "(USA)",
        "DMG-OPE",
        3762665606,
        32768,
        "En",
        "USA"
   , " " },
    {
        "c75c51773c21f3d74f9fca376f1cd75cd2a4b184",
        "Pop Up",
        "(Europe)",
        "DMG-OPX",
        2136234816,
        32768,
        "En",
        "Europe"
   , " " },
    {
        "7e2439e83555e60311744043adb2fff6dacc03c2",
        "Super Off Road",
        "(USA, Europe)",
        "DMG-ORE",
        1042813118,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "2ee30e5cfac3fc43cf81aba0945883f9b9985447",
        "Final Fantasy Legend III",
        "(USA)",
        "DMG-OSE",
        1044727568,
        262144,
        "En",
        "USA"
   , " " },
    {
        "f98eb7794df48fd0c6bac5469cf0219c8773369c",
        "Sa-Ga 3 - Jikuu no Hasha",
        "(Japan)",
        "DMG-OSJ",
        1465740701,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "6c4469f37cc28f4139bde137abaf29305711c955",
        "Othello",
        "(Japan)",
        "DMG-OTJ",
        3245998126,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "104159076a3e3fbddbce923c6b06be918eefa0b1",
        "Othello",
        "(Europe)",
        "DMG-OTX",
        3865083433,
        32768,
        "En",
        "Europe"
   , " " },
    {
        "b63995888f0c806f3b15c7889b14634abfb0a43d",
        "Pri Pri - Primitive Princess!",
        "(Japan)",
        "DMG-P2A",
        1440363797,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "7fa332b0497fcc31dfc9456ecd7757b24f17d0aa",
        "A-mazing Tater",
        "(USA)",
        "DMG-P3E",
        3525946466,
        65536,
        "En",
        "USA"
   , " " },
    {
        "f0cde3d0d707244bfda7d68a43cc9e4483329bf5",
        "Puzzle Boy II",
        "(Japan)",
        "DMG-P3J",
        3846584861,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "2dabd93e21fd45d5d86bec99afb30565bb5d7ac4",
        "Pachio-kun 2",
        "(Japan)",
        "DMG-P4J",
        1049430761,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "ea0814d9669a7120e71c7f6146cae53ea30b5c42",
        "Penta Dragon",
        "(Japan)",
        "DMG-P5J",
        506395374,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "c3e5969e40937a03b9d48e4cdd0caab8df46c12a",
        "Parasol Stars - Rainbow Islands II",
        "(Europe)",
        "DMG-P6X",
        3277508904,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "637355dfb3dca295c5e6e241b64dcffe296da668",
        "Pachinko Kaguya Hime",
        "(Japan)",
        "DMG-P7J",
        3067729604,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "65c823fe4d7b9b028b64d0749d6c7c70c7a44070",
        "Phantasm",
        "(Japan)",
        "DMG-P8J",
        3879884318,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "6ad757b7bab8cb5c83f514201ac00e426d24980e",
        "Avenging Spirit",
        "(USA, Europe)",
        "DMG-P8X",
        3475744247,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "30b17c37cccca5e9f15883e7ce9c084a2057bfbf",
        "Populous Gaiden",
        "(Japan)",
        "DMG-P9A",
        4079425150,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "17c94cf14e98611924eb22c2cf460d46573c64e7",
        "Populous",
        "(Europe)",
        "DMG-P9X",
        1160796076,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "0822cb284da18a27adc9e01b47958159059f7fdf",
        "Pinball - Revenge of the 'Gator",
        "(USA, Europe)",
        "DMG-PBE",
        2038448901,
        65536,
        "En",
        "USA, Europe"
   , " " },
    {
        "1b144dec2d0a9f2339634eca332b1300cba3fc50",
        "Pinball - 66hiki no Wani Daikoushin!",
        "(Japan)",
        "DMG-PBJ",
        456821190,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "07566a5b8b1d6f78bbc4962933f850ce5d4ffab6",
        "Pac-Man",
        "(Japan)",
        "DMG-PCA",
        1704562504,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "1a2c61185cfe2261f4c3a22a7bb3d0a42b8c3ce9",
        "Pocket Camera",
        "(Japan) (Rev 1) (SGB)",
        "GBD-PCAJ",
        1944645526,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "157-47865",
        "Game Boy Camera",
        "(USA, Europe) (SGB)",
        "GBD-PCAX",
        1178636447,
        1048576,
        "En",
        "USA, Europe",
        "/camera64.sprite" 
    },
    {
        "1df6e4ed6577adf31892871eaa3350eb99c9f102",
        "Game Boy Camera Gold",
        "(USA) (SGB)",
        "GBD-PCBX",
        2711877510,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "cc5f46aeecd191204b19af4988771c857fe68796",
        "Pac-Man",
        "(USA)",
        "DMG-PCE",
        3061965379,
        65536,
        "En",
        "USA"
   , " " },
    {
        "84d10d3fecc1d4fcf782267eadfa093ad82a8f2f",
        "Pac-Man",
        "(Europe)",
        "DMG-PCX",
        84477596,
        65536,
        "En",
        "Europe"
   , " " },
    {
        "dd8500cb12db0a11c4f1e31936651e7e48abce41",
        "Pipe Dream",
        "(Japan)",
        "DMG-PDA",
        3635719844,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "6272896c0792190ae10f6b1b5ad6942301a7149b",
        "Pipe Dream",
        "(USA)",
        "DMG-PDE",
        4120702442,
        32768,
        "En",
        "USA"
   , " " },
    {
        "b8ae7288799d103a57d240470b7fe9167e2417f4",
        "Painter Momopie",
        "(Japan)",
        "DMG-PEJ",
        4219072344,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "f808a6923f86ae69c204d77792e72707951333aa",
        "Peetan",
        "(Japan)",
        "DMG-PFJ",
        3489926985,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "5d0c12247f412fdb6af366613506d4796ed0f7ea",
        "Popeye 2",
        "(USA)",
        "DMG-PGE",
        1922838080,
        131072,
        "En",
        "USA"
   , " " },
    {
        "9201159b3c06f8e65a218ff91abd0d46922775f1",
        "Popeye 2",
        "(Japan) (Rev 1)",
        "DMG-PGJ",
        2363045657,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "f61817c2571636fc39c8ce98c9257c6c2b00178b",
        "Popeye 2",
        "(Japan)",
        "DMG-PGJ",
        1131605352,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "2ed7e678462db6501d82e7da9e375a4badaf78e1",
        "Popeye 2",
        "(Europe)",
        "DMG-PGX",
        3229538082,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "53b3be43001f132cf6e6a3e2263463e04da59c14",
        "Parasol Henbee",
        "(Japan)",
        "DMG-PHJ",
        2929192019,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "12c94db5d23a15fc15e278a2aa5ab59cfefdfea4",
        "Higashio Osamu Kanshuu Pro Yakyuu Stadium '91",
        "(Japan)",
        "DMG-PIJ",
        1107853742,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "9b759841f7bc1a27d186a29e0e54d440783521c9",
        "Pachinko Saiyuuki",
        "(Japan)",
        "DMG-PJJ",
        2408781873,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "e99f72389957d94efd5c6314baebd37f776e295d",
        "Pocket Stadium",
        "(Japan)",
        "DMG-PKJ",
        3351089704,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "7619178b3b0ac6f99aff5ee3bc1666669387110c",
        "Penguin Land",
        "(Japan)",
        "DMG-PLJ",
        2850434696,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "5c6614c0f1a6ac2c41b1c8496300a967e9eb0dba",
        "Pitman",
        "(Japan)",
        "DMG-PMA",
        2696315190,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "86c7775d10899ddb7901c9544377f2e36b03bbfa",
        "Catrap",
        "(USA)",
        "DMG-PME",
        2914607440,
        32768,
        "En",
        "USA"
   , " " },
    {
        "056d4da3873ab2324ace7b8830bc081996cf3131",
        "Amazing Penguin",
        "(USA, Europe)",
        "DMG-PNE",
        806475210,
        65536,
        "En",
        "USA, Europe"
   , " " },
    {
        "9d8568ec65cd8c0baf07f1dbfe56251eac96cff4",
        "Osawagase! Penguin Boy",
        "(Japan)",
        "DMG-PNJ",
        1418801703,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "dd2022074e55148bd9d4c720c2813a3015a3d2fb",
        "Hero Shuugou!! Pinball Party",
        "(Japan)",
        "DMG-PPJ",
        1946505965,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "3aac5fd20321443706d6edfdbb20badf6e325b1b",
        "Head On",
        "(Japan)",
        "DMG-PQA",
        2021854639,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "91dc577d138c6b2364adabcad4f43d934696ce36",
        "Power Racer",
        "(USA)",
        "DMG-PQE",
        2949032265,
        65536,
        "En",
        "USA"
   , " " },
    {
        "1dc486ca431449287fd0eb49c1f7a9bae2d0a9cd",
        "Power Racer",
        "(Europe)",
        "DMG-PQX",
        3481988840,
        65536,
        "En",
        "Europe"
   , " " },
    {
        "c02a2987d6e47058f186d29f1f7ad01b4b15b090",
        "HAL Wrestling",
        "(USA)",
        "DMG-PRE",
        1581933011,
        131072,
        "En",
        "USA"
   , " " },
    {
        "9969c4f9d63b2a11aaf546452fac0d9b32f50731",
        "Pro Wrestling",
        "(Japan)",
        "DMG-PRJ",
        4068214490,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "d3870b93bbb314d3fa430e1fa5a0a56cf621cda0",
        "Fushigi na Blobby - Princess Blob o Sukue!",
        "(Japan)",
        "DMG-PSJ",
        1024175120,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "adb1f3b040cb8ebdde44c9429a296d38f24e151e",
        "Pachinko Time",
        "(Japan)",
        "DMG-PTJ",
        1197837022,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "64a7d9e10459ae5cc3f90193a2d214b5a6e011b9",
        "Daedalian Opus",
        "(USA)",
        "DMG-PUE",
        3065323470,
        32768,
        "En",
        "USA"
   , " " },
    {
        "4fd1090e7441fb6c5381c53ed76b6228cd11ad2f",
        "Bouken! Puzzle Road",
        "(Japan)",
        "DMG-PUJ",
        2668574214,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "ba5ff951d92c4f0cf6775dabbc6831fe57be8287",
        "Parodius Da!",
        "(Japan)",
        "DMG-PVJ",
        395438418,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "9da89b01f151b0a02d0385d6477171eff27c9aaf",
        "Parodius",
        "(Europe)",
        "DMG-PVX",
        2239005989,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "3aa5ce9b589739acfb42a16c4a40b946a7d329b8",
        "Penguin Wars",
        "(USA)",
        "DMG-PWE",
        4006606835,
        65536,
        "En",
        "USA"
   , " " },
    {
        "36719427bf7ccca675ae4a9ad33c3b99dca39377",
        "Penguin-kun Wars VS.",
        "(Japan)",
        "DMG-PWJ",
        3244908653,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "3fe512fcfff661f91f2b28a552d79de153f14596",
        "King of the Zoo",
        "(Europe)",
        "DMG-PWX",
        3537258692,
        65536,
        "En",
        "Europe"
   , " " },
    {
        "cec05979da11d8e424ee3013df6b7b28623aee81",
        "Palamedes",
        "(Japan)",
        "DMG-PXA",
        1268333837,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "22ec761a7870a51dc1f4b3a5cdd43d31074f96d6",
        "Palamedes",
        "(Europe)",
        "DMG-PXX",
        1382630113,
        32768,
        "En",
        "Europe"
   , " " },
    {
        "ab11335cff2ca53d27a01fd15a294ebc8df3f00c",
        "Popeye",
        "(Japan)",
        "DMG-PYA",
        800381473,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "36b02dc8855b41ec54ad91e691e751a3d4321d6b",
        "Puzznic",
        "(Japan)",
        "DMG-PZA",
        2439930765,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "98ef857b52c6de9ef8e98bfd87ee814640c5109d",
        "Super Chase H.Q.",
        "(USA, Europe)",
        "DMG-Q2E",
        1661865472,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "c23dfa7e6556ce05b5cbe95c4b7da9177bf2f3d4",
        "Cliffhanger",
        "(USA, Europe)",
        "DMG-Q5E",
        2853385273,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "246b936e8a90f46c02d973f83af4c028c9e6ebc4",
        "NFL Quarterback Club",
        "(USA, Europe)",
        "DMG-Q6E",
        3347286064,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "5d04218470e3cf30332f230e2d84f44cac852332",
        "Bishoujo Senshi Sailor Moon R",
        "(Japan)",
        "DMG-QAJ",
        3933575285,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "7c362dfa8ea329138f542ff65d58f1296c413614",
        "Q Billion",
        "(Japan)",
        "DMG-QBA",
        2543767892,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "e023326db66b16a22365bbec0f2e41cb118274c0",
        "Q Billion",
        "(USA)",
        "DMG-QBE",
        1912793459,
        32768,
        "En",
        "USA"
   , " " },
    {
        "0ef8d1fc99815360e7c7cdeb7183b2b75b1760ff",
        "Chuck Rock",
        "(USA, Europe)",
        "DMG-QCE",
        3314883998,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "4493727a94801a1062982a5a067a0fe9012134cf",
        "Donkey Kong",
        "(World) (Rev 1) (SGB)",
        "DMG-QDA",
        4151813592,
        524288,
        "En",
        "World"
   , " " },
    {
        "fca22343d4530ff65f4f6948313a5203c2b5e40c",
        "Donkey Kong",
        "(Japan, USA) (En) (SGB)",
        "DMG-QDA",
        3987420024,
        524288,
        "En",
        "Japan, USA"
   , " " },
    {
        "622a1f684985e276e77b908f6d052feb188a931c",
        "Kachiuma Yosou Keiba Kizoku EX '94",
        "(Japan)",
        "DMG-QEJ",
        3560018281,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "3c4ed01387b01c8fa0ff6978172b51027b6963c9",
        "Pit-Fighter",
        "(USA, Europe)",
        "DMG-QFE",
        109100995,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "c87c5c853383ca25ecb29ee09f18497c1618c8eb",
        "Kizuchi da Quiz da Gen-san da!",
        "(Japan)",
        "DMG-QGJ",
        1956979609,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "59d2005d6538222e9cb58791e83c933c5811d949",
        "Castle Quest",
        "(Europe)",
        "DMG-QHX",
        797618031,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "e6cb1d7c7e335a1759b91924618be170f78ca8e6",
        "Sports Illustrated - Championship Football & Baseball",
        "(USA)",
        "DMG-QME",
        592540100,
        524288,
        "En",
        "USA"
   , " " },
    {
        "3b96e3438271e9ca05b18530798994893e60ad5e",
        "Monster Maker - Barcode Saga",
        "(Japan)",
        "DMG-QNJ",
        1455057521,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "e1d76c446b8c49dbca1d3d6bcfe9170a2dd7a9ce",
        "Cool Spot",
        "(USA)",
        "DMG-QPE",
        2879511241,
        131072,
        "En",
        "USA"
   , " " },
    {
        "835f2db17d8a73dd218a5466e47c47eae8594e1d",
        "Cool Spot",
        "(Europe)",
        "DMG-QPX",
        428337868,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "2f06b2b2110f99c7841cee53680c643ece6031e7",
        "Puyo Puyo",
        "(Japan) (Rev 1) (SGB)",
        "DMG-QQJ",
        551846533,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "340f7c39ed020a3309c3fb4c6309c20cd77bf6ec",
        "Puyo Puyo",
        "(Japan) (SGB)",
        "DMG-QQJ",
        2654416147,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "f0576284674af211bea5cc367a286d708c0006f0",
        "Quarth",
        "(USA, Europe)",
        "DMG-QRE",
        3216052909,
        65536,
        "En",
        "USA, Europe"
   , " " },
    {
        "d3f3ac1bafea1b2cc2902476dc3b532626b35985",
        "Quarth",
        "(Japan)",
        "DMG-QRJ",
        1088794057,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "5ce00af993306af5ef9cb495fa4bc169dde60dc2",
        "Quiz Sekai wa Show by Shoubai!!",
        "(Japan)",
        "DMG-QSJ",
        3636673997,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "c0feb3d6f67cdca6fa806a0cc007f5c96da8330d",
        "Q-bert for Game Boy",
        "(USA, Europe)",
        "DMG-QTE",
        2953876805,
        65536,
        "En",
        "USA, Europe"
   , " " },
    {
        "a9334defd75017baf0932ece49fef89ff4f608ac",
        "Q-bert for Game Boy",
        "(Japan)",
        "DMG-QTJ",
        3921300397,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "47633edeb3f4a725671f99ae68caea19d8419df8",
        "Ultraman Chou Toushi Gekiden",
        "(Japan) (SGB)",
        "DMG-QUJ",
        3104056077,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "7140d09d452a37ddceef2a029731af17b9f7f103",
        "QIX",
        "(World)",
        "DMG-QXA",
        2441472158,
        65536,
        "En",
        "World"
   , " " },
    {
        "0216f4673543b871706520371b358007b4b638f4",
        "RoboCop 2",
        "(USA, Europe)",
        "DMG-R2E",
        1130735497,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "3af9877762a6a2b3dd65feb6a2dd531b0e4992eb",
        "RoboCop 2",
        "(Japan)",
        "DMG-R2J",
        360507109,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "d48275e3d6a6b1a417fef7e89f5898523a93e088",
        "Ranma 1-2 - Netsuretsu Kakutou Hen",
        "(Japan)",
        "DMG-R3J",
        3654644049,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "e45a68a3d1fe300baa4c8a99e5b73d36663a1d94",
        "Mega Man IV",
        "(USA)",
        "DMG-R4E",
        2882445325,
        524288,
        "En",
        "USA"
   , " " },
    {
        "d1c8c050a52351401e583c5848413305ad8774bb",
        "Rockman World 4",
        "(Japan)",
        "DMG-R4J",
        380552537,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "f61d618895bbac304ecd1eaadd81b770e9bc3dcf",
        "Mega Man IV",
        "(Europe)",
        "DMG-R4X",
        101419494,
        524288,
        "En",
        "Europe"
   , " " },
    {
        "5951cb14953694a005e40137a80ecfe8a8c31639",
        "Splitz - Nigaoe 15 Game",
        "(Japan)",
        "DMG-R5A",
        2960795461,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "47c33531240acc6110bf2403ac4adb8c17e54b91",
        "Splitz",
        "(Europe)",
        "DMG-R5X",
        287606729,
        65536,
        "En",
        "Europe"
   , " " },
    {
        "29cc44b1fd73a999cd407a5e827c8ab2fcd53d5b",
        "Kamen Rider SD - Hashire! Mighty Riders",
        "(Japan)",
        "DMG-R6J",
        1908933811,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "2c2ee021743b2b6eab19c2b7ee04ca966e17eddf",
        "Battletoads in Ragnarok's World",
        "(USA)",
        "DMG-R7E",
        3459345512,
        131072,
        "En",
        "USA"
   , " " },
    {
        "2993be667f29566a718f83f94d06f263557c5077",
        "Battletoads in Ragnarok's World",
        "(Europe)",
        "DMG-R7X",
        2147235050,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "3933d1471819a91b5f225f61100619b74e7619ea",
        "Rampart",
        "(USA, Europe)",
        "DMG-R8E",
        1832155154,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "108de50bed8f428e5aba5216cf8b90a42a2fc18f",
        "Rampart",
        "(Japan)",
        "DMG-R8J",
        2527381280,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "d4d4cd1b78cf5492503a7dbc9a00ebadad3ae2ce",
        "GB Genjin Land - Viva! Chikkun Oukoku",
        "(Japan)",
        "DMG-R9J",
        2961944854,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "cd9c185855f2a2ce3bd3503625fb3c59a57df1cd",
        "Gargoyle's Quest",
        "(Europe) (Rev 1)",
        "DMG-RAE",
        3094280101,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "eeb4dbc75d37001223bab704a92fcae1ef6b93f4",
        "Gargoyle's Quest",
        "(USA, Europe)",
        "DMG-RAE",
        3204639,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "61df4e6f2c86ab9ecc6032692e7e22ff20d42415",
        "Red Arremer - Makaimura Gaiden",
        "(Japan)",
        "DMG-RAJ",
        2919309994,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "9e85f8e94be87e6fed9b82aea2630336a2d9d21c",
        "Super R.C. Pro-Am",
        "(USA, Europe)",
        "DMG-RCE",
        2036396863,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "371591e7bb5584479f0583774c1b02c28ccd7d41",
        "Racing Damashii",
        "(Japan)",
        "DMG-RDJ",
        2037365606,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "5225420d1749c607e36475199908c590f4ded930",
        "R-Type",
        "(Japan)",
        "DMG-REA",
        3835201808,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "f3d6985935508672149aac78b547a04fab83e40f",
        "R-Type",
        "(USA, Europe)",
        "DMG-REE",
        3773972416,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "2a8789008fd79ffb7dc0de1a8b4e8a1f1ed57a94",
        "Makaimura Gaiden - The Demon Darkness",
        "(Japan)",
        "DMG-RFJ",
        3483588830,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "79e03f896fb0789694206c6fb6dcceb3b9b6262f",
        "Zettai Muteki Raijin-Oh",
        "(Japan)",
        "DMG-RGJ",
        2097377917,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "9063bfe134b1d1119efac8e67985af18311e801c",
        "Robin Hood - Prince of Thieves",
        "(Germany)",
        "DMG-RHD",
        186675619,
        262144,
        "De",
        "Germany"
   , " " },
    {
        "9c28d42449ecf2abcf8527f159bffb753826dcb0",
        "Robin Hood - Prince of Thieves",
        "(USA)",
        "DMG-RHE",
        3855139505,
        262144,
        "En",
        "USA"
   , " " },
    {
        "24aa2874966dfd42693c314637695cbdc6011fbc",
        "Robin Hood - Prince of Thieves",
        "(France)",
        "DMG-RHF",
        4011477306,
        262144,
        "Fr",
        "France"
   , " " },
    {
        "d8fbc09f862472ce906bdd3084c42ae74220cb6e",
        "Robin Hood - Prince of Thieves",
        "(Spain)",
        "DMG-RHS",
        2017910653,
        262144,
        "Es",
        "Spain"
   , " " },
    {
        "251772d53a13a76551220133532f95fa66d3e17b",
        "Robin Hood - Prince of Thieves",
        "(Europe)",
        "DMG-RHX",
        1438360707,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "840aac4527a01408c2bf70a7f282e2183eb7e5b3",
        "Dirty Racing",
        "(Japan)",
        "DMG-RIJ",
        1135560113,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "dbca2d24dfd4a6898d7d15e79f23e3ab38c8acd0",
        "Bonk's Revenge",
        "(USA) (SGB)",
        "DMG-RJE",
        4046736248,
        262144,
        "En",
        "USA"
   , " " },
    {
        "fd7214d1cffbb5dd24dea91f7358118c164fbe06",
        "GB Genjin 2",
        "(Japan) (SGB)",
        "DMG-RJJ",
        1870321113,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "1fa5d294daeaac6c4ea53021bf612e570b54dc7d",
        "B.C. Kid 2",
        "(Europe) (SGB)",
        "DMG-RJX",
        3264147554,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "6979650bfc1ce84ccec75807b9d5b8a3ba8434e5",
        "Star Trek - 25th Anniversary",
        "(USA, Europe)",
        "DMG-RKE",
        1616766010,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "7b4831cd013625fdc2f7378e63986d9012d7048b",
        "Yousei Monogatari - Rod Land",
        "(Japan)",
        "DMG-RLJ",
        592821908,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "d39b0a8da30ea4291a6cf82c2a91668fc54eb8a0",
        "Rodland",
        "(Europe)",
        "DMG-RLX",
        1620309228,
        65536,
        "En",
        "Europe"
   , " " },
    {
        "6b060f5caed156b547eaa6770a719d2a71afb7c5",
        "Kaisen Game - Radarmission",
        "(Japan)",
        "DMG-RMA",
        2301608111,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "9d5eb0ecf06e029796f571f4364660de69b7fd83",
        "Radar Mission",
        "(USA, Europe)",
        "DMG-RME",
        1478339017,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "488394514f7dce96ce5b51870cc781f2ec8db68c",
        "Ranma 1-2",
        "(Japan)",
        "DMG-RNJ",
        1308971902,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "2bfbd3400a87da905d4e7996722142e236d65fb3",
        "Ranma 1-2 - Kakugeki Mondou!!",
        "(Japan)",
        "DMG-ROJ",
        1631479162,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "aeecc966a0c91d3c3bc133aaaa5aa90fb7a71931",
        "David Crane's The Rescue of Princess Blobette",
        "(USA)",
        "DMG-RPE",
        2182127679,
        65536,
        "En",
        "USA"
   , " " },
    {
        "b3f2e20faaa2043445c8b3b55ffc3afbc3fe9ff5",
        "David Crane's The Rescue of Princess Blobette",
        "(Europe)",
        "DMG-RPX",
        637021105,
        65536,
        "En",
        "Europe"
   , " " },
    {
        "20cdd487841095879ba2e62ddb9776c6e87e481f",
        "Who Framed Roger Rabbit",
        "(USA)",
        "DMG-RRE",
        3107010810,
        131072,
        "En",
        "USA"
   , " " },
    {
        "b29e8e636a1959239a27b719a06fa89df227051f",
        "Who Framed Roger Rabbit",
        "(Spain)",
        "DMG-RRS",
        3440494871,
        131072,
        "Es",
        "Spain"
   , " " },
    {
        "0405ea30bf598c0d3daf36deb45266b1df23fc3d",
        "Who Framed Roger Rabbit",
        "(Europe)",
        "DMG-RRX",
        3689863246,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "122969d99b654541cc8d60f9b06860bfe49f8caf",
        "Roadster",
        "(Japan)",
        "DMG-RSA",
        71646840,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "d6635df1b31572bac0a8eb10856fc61113326fcc",
        "Ray-Thunder",
        "(Japan)",
        "DMG-RTJ",
        2685047912,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "3e61996de6e9076f38c0389ceb7f31f9a220f925",
        "Ring Rage",
        "(Japan)",
        "DMG-RUA",
        4248656807,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "eaf64e320e502630253cc9a7b1bd2dbe9737e1c5",
        "Ring Rage",
        "(USA)",
        "DMG-RUE",
        3169181623,
        131072,
        "En",
        "USA"
   , " " },
    {
        "de31e0731108e789aed98185a3c94fc2b7b7fb11",
        "Race Drivin'",
        "(USA, Europe)",
        "DMG-RVE",
        1735773398,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "de285127fb657521c5928292357eec819f7e53b7",
        "Rockman World",
        "(Japan)",
        "DMG-RWA",
        1004973060,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "f0e031d20c26e0a01e757f12a878903b3ac849fb",
        "Mega Man - Dr. Wily's Revenge",
        "(USA)",
        "DMG-RWE",
        1206324744,
        262144,
        "En",
        "USA"
   , " " },
    {
        "20c412ccbdfad4bb9d87e48a9b414f2487422bef",
        "Mega Man - Dr. Wily's Revenge",
        "(Europe)",
        "DMG-RWX",
        782547318,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "3948607842f8db1f42ca2b673ce56c1e65db9d45",
        "Ren & Stimpy Show, The - Space Cadet Adventures",
        "(USA)",
        "DMG-RXE",
        545630937,
        262144,
        "En",
        "USA"
   , " " },
    {
        "cee37c82a82f14164d508c67ea4478fa59fb9a3e",
        "Adventures of Rocky and Bullwinkle and Friends, The",
        "(USA)",
        "DMG-RYE",
        908887068,
        131072,
        "En",
        "USA"
   , " " },
    {
        "8e4ee5f30b9298ef2257b14788d70125ccbced0f",
        "R-Type II",
        "(Japan)",
        "DMG-RZA",
        1610801809,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "1180d7ac99a6d6a5c2af4ebb476338167472160c",
        "R-Type II",
        "(Europe)",
        "DMG-RZX",
        803383085,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "d9377dd5e10bad5469b6f3c78f82e5c9c8d35571",
        "Final Fantasy Legend II",
        "(USA)",
        "DMG-S2E",
        1479623042,
        262144,
        "En",
        "USA"
   , " " },
    {
        "2cfcd38528dff6f81dc4348e4da0378f4ef9c112",
        "Sa-Ga 2 - Hihou Densetsu",
        "(Japan) (Rev 1)",
        "DMG-S2J",
        4140814257,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "db91d6ce5d09098f2f274c7f2399398e989d74d9",
        "Sa-Ga 2 - Hihou Densetsu",
        "(Japan)",
        "DMG-S2J",
        403004345,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "16072e9bda52f53125252066e24ed7f92bb746af",
        "Spot",
        "(Japan)",
        "DMG-S3A",
        749044352,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "7597fb97e51599732e624e89bff92e3d26093f04",
        "Spot - The Video Game",
        "(USA)",
        "DMG-S3E",
        4241890200,
        32768,
        "En",
        "USA"
   , " " },
    {
        "f7ed558fa18a16a754594375efcb80a3cba17b36",
        "Spot - The Video Game",
        "(Europe)",
        "DMG-S3X",
        81936868,
        32768,
        "En",
        "Europe"
   , " " },
    {
        "0e3254840b7fcdf33ed77f7257e88d7789a1143d",
        "Tsumeshougi - Hyakuban Shoubu",
        "(Japan)",
        "DMG-S4J",
        1138927040,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "1aa663434ffc3134093c69c435b1dca8850ed570",
        "Solitaire",
        "(Japan)",
        "DMG-S5A",
        286869578,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "3c282db4ecaa8d8f3a05ba8e56d415cf59127112",
        "Lazlos' Leap",
        "(USA)",
        "DMG-S5E",
        838549579,
        65536,
        "En",
        "USA"
   , " " },
    {
        "41037b984d24752e8c2b16f5d2e449b544d819f1",
        "Sanrio Carnival 2",
        "(Japan)",
        "DMG-S6J",
        1972140842,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "69af22f0c3c54e644276aa524641ace298611255",
        "Saint Paradise - Saikyou no Senshi-tachi",
        "(Japan)",
        "DMG-S7J",
        3606985814,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "8d64eb101fb6d0064525e482e3e1efa407f0d0f8",
        "Sagaia",
        "(Japan)",
        "DMG-S8A",
        3829244048,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "a192ba6931406b93150ddbfd4ebc7de914b53262",
        "Soldam",
        "(Japan)",
        "DMG-S9J",
        2086333574,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "ee45e794956972212783a8caa0651a278d1e2797",
        "Final Fantasy Legend, The",
        "(USA)",
        "DMG-SAE",
        2152076431,
        131072,
        "En",
        "USA"
   , " " },
    {
        "3fa152aac8a20f7762d30c2c043366b7eea974f9",
        "Makai Toushi Sa-Ga",
        "(Japan) (Rev 1)",
        "DMG-SAJ",
        424903183,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "e95cd1545ba34bd5e078c4a7d4742df3b1ea9b6f",
        "Makai Toushi Sa-Ga",
        "(Japan)",
        "DMG-SAJ",
        320539122,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "7c3226f1374b6dbe7a824ec70f76a3a98270acca",
        "Soccer Boy",
        "(Japan)",
        "DMG-SBA",
        603344514,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "b47cf53770b471e6b8a3ee6d1dd1e803345bce08",
        "Soccer Mania",
        "(USA)",
        "DMG-SBE",
        1871164739,
        65536,
        "En",
        "USA"
   , " " },
    {
        "d52ab182ea7051126048665aca972f7bc593a78e",
        "Solomon's Club",
        "(USA)",
        "DMG-SCE",
        3467207210,
        65536,
        "En",
        "USA"
   , " " },
    {
        "f61bcc1125f99223d083cea309e6f305703f1b05",
        "Solomon's Club",
        "(Japan)",
        "DMG-SCJ",
        2417753902,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "9a2b1d8305becc8c83ff79141fb0fa78b422b009",
        "Solomon's Club",
        "(Europe)",
        "DMG-SCX",
        1880935859,
        65536,
        "En",
        "Europe"
   , " " },
    {
        "5a26b928f6815a1b532498ce929be66a2ddf2611",
        "SD Gundam - SD Sengokuden - Kunitori Monogatari",
        "(Japan)",
        "DMG-SDJ",
        23725394,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "e18ec6a3707237161eba2370115784414567771f",
        "Sword of Hope, The",
        "(Germany)",
        "DMG-SED",
        1234046623,
        131072,
        "De",
        "Germany"
   , " " },
    {
        "5a094f60bc66c6aa4fde661a5039feeec838958f",
        "Sword of Hope, The",
        "(USA)",
        "DMG-SEE",
        2123511878,
        131072,
        "En",
        "USA"
   , " " },
    {
        "bd272ff855cdcfe13ae4c9c2ec9512f13e464b17",
        "Selection - Erabareshi Mono",
        "(Japan)",
        "DMG-SEJ",
        2040290523,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "a4624400257a9858aa7e3d58e327d4a2e688a3a2",
        "Sword of Hope, The",
        "(Spain)",
        "DMG-SES",
        683872727,
        131072,
        "Es",
        "Spain"
   , " " },
    {
        "16eecfd90c2c9c60fe847d99058de9c80b888948",
        "Sword of Hope, The",
        "(Sweden)",
        "DMG-SEW",
        3353644156,
        131072,
        "Sv",
        "Sweden"
   , " " },
    {
        "1319b032d26e65d95e5e8b3ead1053499c5404b8",
        "Sumo Fighter",
        "(USA)",
        "DMG-SFE",
        3135012078,
        131072,
        "En",
        "USA"
   , " " },
    {
        "2f6d131bd0d221226f71e0af3ae292563a01c888",
        "Sumou Fighter - Toukaidou Basho",
        "(Japan)",
        "DMG-SFJ",
        2794479538,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "3ecafc9791ff5eeae36032ac5ddfe5ee1071ddc7",
        "Shougi",
        "(Japan)",
        "DMG-SGJ",
        1337216180,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "9a5488070f2cada87426e971a78e9b0a2bf26c1c",
        "Shanghai",
        "(USA)",
        "DMG-SHE",
        1477430040,
        32768,
        "En",
        "USA"
   , " " },
    {
        "aed431122e32b37b1737da1f2e6c57a469da0c82",
        "Shanghai",
        "(Japan) (HAL Laboratory)",
        "DMG-SHJ",
        2083088171,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "85692aced8a7bfb79f497940933c9012a65c7c6c",
        "Selection II - Ankoku no Fuuin",
        "(Japan)",
        "DMG-SIJ",
        3499127798,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "a85170ee8b8c54acb32c2dd99400e303fb03018c",
        "Sword of Hope II, The",
        "(USA)",
        "DMG-SIX",
        1535112076,
        262144,
        "En",
        "USA"
   , " " },
    {
        "4cc1e9e1618126ef1ac83fc921f0fded402525b3",
        "Snow Brothers",
        "(USA)",
        "DMG-SJE",
        3562675626,
        131072,
        "En",
        "USA"
   , " " },
    {
        "8519f60e5f38bb2b589a10810d6ec98f5f5da1d5",
        "Snow Bros. Jr.",
        "(Japan)",
        "DMG-SJJ",
        3852611418,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "358f54966359022192bcc95fd457b3912f97c23d",
        "Snow Brothers",
        "(Europe)",
        "DMG-SJX",
        2417275864,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "982cf1851599ae7a1e0bd8869826c7681236b75c",
        "Skate or Die - Bad 'N Rad",
        "(USA)",
        "DMG-SKE",
        1303889943,
        131072,
        "En",
        "USA"
   , " " },
    {
        "0497b589934dad8ac3356a84b4334c385b57a32d",
        "Skate or Die - Bad 'N Rad",
        "(Europe)",
        "DMG-SKX",
        2887758458,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "db999b66a78cbd52826eeb80fbb5615ec527e8b3",
        "InfoGenius Productivity Pak - Spell Checker and Calculator",
        "(USA)",
        "DMG-SLE",
        1967429307,
        131072,
        "En",
        "USA"
   , " " },
    {
        "78cbae71ba90ed689b1cd20ac2837d2e3101e576",
        "Amazing Spider-Man, The",
        "(USA, Europe)",
        "DMG-SME",
        709799908,
        65536,
        "En",
        "USA, Europe"
   , " " },
    {
        "7910362d88a48ddd319649af854f8bf177e55861",
        "Snoopy - Magic Show",
        "(Japan)",
        "DMG-SNA",
        1217566797,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "ae4df7b01e70afc2646fb6a7f3e5408c63a4dce8",
        "Snoopy's Magic Show",
        "(USA, Europe)",
        "DMG-SNE",
        729436212,
        65536,
        "En",
        "USA, Europe"
   , " " },
    {
        "48cb9e0f056f9ba9b412c251d8523ae5f53cca26",
        "Boxxle",
        "(USA, Europe) (Rev 1)",
        "DMG-SOE",
        3231510169,
        32768,
        "En",
        "USA, Europe"
   , " " },
    {
        "f4896339f6410fa1e62d2be27154493b81806d5d",
        "Boxxle",
        "(USA)",
        "DMG-SOE",
        612644967,
        32768,
        "En",
        "USA"
   , " " },
    {
        "a4c120649698d22c1478f304b6cc32b1e2cd7be5",
        "Soukoban",
        "(Japan)",
        "DMG-SOJ",
        3574426890,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "eba45a1a51a3abacaf42fa1413f5646bfe822b5b",
        "Space Invaders",
        "(Japan)",
        "DMG-SPA",
        2257278898,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "c908e69b38b32af64ded2bafa9ebbc7d1a9b79be",
        "Space Invaders",
        "(USA) (SGB)",
        "DMG-SPE",
        2131711842,
        524288,
        "En",
        "USA"
   , " " },
    {
        "25ae264f597e91cb56cc1e03b750fac83a79b184",
        "Space Invaders",
        "(Europe) (SGB)",
        "DMG-SPX",
        2300284676,
        524288,
        "En",
        "Europe"
   , " " },
    {
        "10db15335ac81b31b85b6b7b1458f1acece14dfd",
        "Boxxle II",
        "(USA, Europe)",
        "DMG-SQE",
        3230570326,
        32768,
        "En",
        "USA, Europe"
   , " " },
    {
        "73d0a1ae324e15cb5fac18748f0199e9d2bc7547",
        "Soukoban 2",
        "(Japan)",
        "DMG-SQJ",
        3654969153,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "571c07bddc0faa51f5c1120921e2e91eeee04dae",
        "Super Scrabble",
        "(USA)",
        "DMG-SRE",
        947173282,
        131072,
        "En",
        "USA"
   , " " },
    {
        "186dec7f62b154e81c20f4c76b41016cc526b4ee",
        "SolarStriker",
        "(World)",
        "DMG-SSA",
        293695747,
        65536,
        "En",
        "World"
   , " " },
    {
        "65d2636eba0cd15fec1431234079869aba5fa532",
        "Ishido - The Way of Stones",
        "(USA)",
        "DMG-STE",
        2243529335,
        65536,
        "En",
        "USA"
   , " " },
    {
        "96b54268d50ccdc312875893d494b679ecff323b",
        "Ishido - The Way of Stones",
        "(Japan)",
        "DMG-STJ",
        2872473877,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "3f86ec801b8e2927fe01cfd662ab971ab512b183",
        "Shikinjou",
        "(Japan)",
        "DMG-SUJ",
        4176230977,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "3e7cf222d8857d0cdd5002ae1fb72e1021dfaa0a",
        "Malibu Beach Volleyball",
        "(USA)",
        "DMG-SVE",
        3752188434,
        65536,
        "En",
        "USA"
   , " " },
    {
        "1b9851abd3265eeb50b26be7bef1d851fa0d99c5",
        "Seaside Volley",
        "(Japan)",
        "DMG-SVJ",
        2041570718,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "757c08a2ccaafeaa7a09b143c327f8d285914bbf",
        "Side Pocket",
        "(World)",
        "DMG-SWA",
        2585669812,
        65536,
        "En",
        "World"
   , " " },
    {
        "845a5ff0139ae3c9cf17ff4c3d8f4891b7efb66f",
        "Kung-Fu Master",
        "(USA, Europe)",
        "DMG-SXE",
        859891200,
        65536,
        "En",
        "USA, Europe"
   , " " },
    {
        "29b80ae3516361fbe74e8a2ca5b676249b0ac4fc",
        "Spartan X",
        "(Japan)",
        "DMG-SXJ",
        3397075497,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "39b649dec6d2c580588d64bb19af6a87ae5e734a",
        "Scotland Yard",
        "(Japan)",
        "DMG-SYJ",
        1242519922,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "04c1dd75cfcc2dc9dea9b05bd01ec6e4b8602d18",
        "Super Robot Taisen",
        "(Japan)",
        "DMG-SZJ",
        2111428566,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "e12b400247c01529a3df959e4b1532cb53c32498",
        "Trump Boy II",
        "(Japan)",
        "DMG-T2J",
        957708361,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "66-37985",
        "Adventure Island",
        "(USA, Europe)",
        "DMG-T3E",
        1693776452,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "fe4b2dae7bab2cc52a10e2f4d94eab7dff92ae0a",
        "Takahashi Meijin no Bouken-jima II",
        "(Japan)",
        "DMG-T3J",
        4043775496,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "bbe12b1ccb2b44036d8a9d1fb0fcc94e84589210",
        "Trip World",
        "(Japan)",
        "DMG-T4A",
        290885220,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "c903c536b978c90fad801456a2e28b7a9ac74c05",
        "Trip World",
        "(Europe)",
        "DMG-T4X",
        2245595321,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "18346faa2cbf69be9ffbbbfa6455dbfcb37db65d",
        "Captain Tsubasa VS",
        "(Japan)",
        "DMG-T5J",
        2811619409,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "9ef3a06a8f289ff53474b6ec64f0c1aaa63747c4",
        "Tumble Pop",
        "(Japan)",
        "DMG-T6A",
        2283470178,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "1dcf2de52b30a372d35e7bb08d3b7c2e705c444b",
        "Tumble Pop",
        "(USA, Europe)",
        "DMG-T6E",
        2319100721,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "6c11dca9c9274d800774a0b9d6f30666ec939944",
        "Uchuu no Kishi Tekkaman Blade",
        "(Japan)",
        "DMG-T7J",
        584616094,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "809a70579e5fe3990fa25d1a61aeb5cd9972e0f7",
        "Tiny Toon Adventures 2 - Montana's Movie Madness",
        "(USA, Europe)",
        "DMG-T8E",
        3318187661,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "5cd8f4984d4e813708c931cd01f372769f52d01d",
        "Tiny Toon Adventures 2 - Buster Bunny no Kattobi Daibouken",
        "(Japan)",
        "DMG-T8J",
        265584522,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "6509335c02043673b94d7444afc9fa58cde29396",
        "Gerry Anderson's Thunderbirds",
        "(Japan)",
        "DMG-T9J",
        345936787,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "f83648f84ba47522911f3cccec7ed49b0189a8c3",
        "Tasmania Story",
        "(USA)",
        "DMG-TAE",
        2738638847,
        32768,
        "En",
        "USA"
   , " " },
    {
        "2cc47529f1e1bbbbb1ae48da76e3b5964f36d048",
        "Tasmania Monogatari",
        "(Japan)",
        "DMG-TAJ",
        2661836711,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "be640d254a31d922a9628d42ded5806bbd477fb6",
        "Trump Boy",
        "(Japan)",
        "DMG-TBJ",
        4246121541,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "bf3d725934becc45d5f81fc505336d952bd392d8",
        "Taikyoku Renju",
        "(Japan) (En,Ja)",
        "DMG-TCA",
        1654857190,
        65536,
        "En,Ja",
        "Japan"
   , " " },
    {
        "7628186aea47f0d8ff14364d5d8d28e7288239fc",
        "TwinBee da!!",
        "(Japan)",
        "DMG-TDJ",
        25155317,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "bb070e7e2f62ec6c32e5e98568f8e60700e3980a",
        "Pop'n TwinBee",
        "(Europe)",
        "DMG-TDX",
        3497898612,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "d36228352e333bba13afda30da36fa681ca4437d",
        "Torpedo Range",
        "(Japan)",
        "DMG-TEA",
        531847301,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "45e1d083e456f250b0efe277f0d071965f8f7e68",
        "Torpedo Range",
        "(USA)",
        "DMG-TEE",
        3986195113,
        131072,
        "En",
        "USA"
   , " " },
    {
        "1b3175a15243015393e411a9b13c6fc4ff864bfe",
        "Heisei Tensai Bakabon",
        "(Japan)",
        "DMG-TFJ",
        2885963418,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "4dc840e66c7b8981a04c6cb4979b5fbd17f98fe5",
        "Tail 'Gator",
        "(USA, Europe)",
        "DMG-TGE",
        3316436604,
        65536,
        "En",
        "USA, Europe"
   , " " },
    {
        "d8babd33e137a01d2ebdedc6b7c33379862d920c",
        "Shippo de Bun",
        "(Japan)",
        "DMG-TGJ",
        1447101506,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "876aeb30f6d0d5437dbb0a19ee307fad4562c7a8",
        "Days of Thunder",
        "(USA, Europe)",
        "DMG-THE",
        903462414,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "cc77966a8a00f338e1b67b96737a1997cc90d33b",
        "Tokio Senki - Eiyuu Retsuden",
        "(Japan)",
        "DMG-TIJ",
        2646072515,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "5d98865f80a23ce1aec4ef5422afa1303055e2b8",
        "Yomihon Yumegoyomi - Tenjin Kaisen 2",
        "(Japan)",
        "DMG-TJJ",
        555501073,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "8acacfb2f98f5c89277401c2a6c40dc04c0fa006",
        "Mercenary Force",
        "(USA, Europe)",
        "DMG-TKE",
        2612378115,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "e10841cdd580d8cc92a93652ebd459c104308330",
        "Tenjin Kaisen",
        "(Japan)",
        "DMG-TKJ",
        3071514278,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "8b885452a4ea84c6f18e74e86342c904edb3686e",
        "Tecmo Bowl GB",
        "(Japan)",
        "DMG-TLA",
        3486993972,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "10cda771e42304083ce567d33b4622e190ff2173",
        "Tecmo Bowl",
        "(USA)",
        "DMG-TLE",
        3841103712,
        262144,
        "En",
        "USA"
   , " " },
    {
        "c860b8cc3cf9b85b698dbe5fd905e1d7ee2a418f",
        "Track Meet - Mezase! Barcelona",
        "(Japan)",
        "DMG-TMA",
        1221819602,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "ad96da86f0267fe52eb113bf1d461dd6b1badb67",
        "Litti's Summer Sports",
        "(Germany)",
        "DMG-TMD",
        3867228507,
        131072,
        "De",
        "Germany"
   , " " },
    {
        "09d762748e8f32c4906482154cd62176b765df68",
        "Track Meet",
        "(USA, Europe)",
        "DMG-TME",
        2069742564,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "4dd7bd97097a891c6ba516ecd3b4abab0b878e9a",
        "Tennis",
        "(World)",
        "DMG-TNA",
        1342775647,
        32768,
        "En",
        "World"
   , " " },
    {
        "aa4c66680f5bc301345a6d4d49d3cc23914e7744",
        "Spy vs Spy - Operation Boobytrap",
        "(USA)",
        "DMG-TOE",
        1832969634,
        131072,
        "En",
        "USA"
   , " " },
    {
        "1327e37ca42ff3c117ec8f8a47163fecd38994e5",
        "Trappers Tengoku - Spy vs Spy",
        "(Japan)",
        "DMG-TOJ",
        2951729285,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "0e61214bd8d966bb2b95ccc845478e20d2d79003",
        "Spy vs Spy - Operation Boobytrap",
        "(Europe)",
        "DMG-TOX",
        912544259,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "5c5b6539564f8d8f4a2051ea59a5faa3f5bda42f",
        "Trax",
        "(USA, Europe)",
        "DMG-TPE",
        1245232765,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "c52c2d2dd1c0a2f63e4d1babe2f1e55ebbee1168",
        "Totsugeki! Ponkotsu Tank",
        "(Japan)",
        "DMG-TPJ",
        2606463999,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "5861bd2a5ffcc0b51e2a5769b18d99fb0b92f2d7",
        "Turrican",
        "(USA, Europe)",
        "DMG-TQE",
        593084172,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "10-5823",
        "Tetris",
        "(World) (Rev 1)",
        "DMG-TRA",
        1189056941,
        32768,
        "En",
        "World",
        "/tetris64.sprite" },
    {
        "11-35253",
        "Tetris",
        "(Japan) (En)",
        "DMG-TRA",
        1677279357,
        32768,
        "En",
        "Japan",
        "/tetris64.sprite" },
    {
        "4f2d4dafeaac513e8d979c2c020b291f81b89502",
        "Bart no Survival Camp",
        "(Japan)",
        "DMG-TSA",
        265710362,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "bf4495f7540bea9d194a28cecd66737e2a126b09",
        "Bart Simpson's Escape from Camp Deadly",
        "(USA, Europe)",
        "DMG-TSE",
        1430692738,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "4ad99a2107ddf758bf02f5e6791ebbcbe6f4b649",
        "Teenage Mutant Ninja Turtles 2",
        "(Japan)",
        "DMG-TTA",
        2229730085,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "a6706c5335cbbd6a067d566c1cc82456f929ddf3",
        "Teenage Mutant Ninja Turtles II - Back from the Sewers",
        "(USA)",
        "DMG-TTE",
        1156914291,
        262144,
        "En",
        "USA"
   , " " },
    {
        "a7aac037ed2f3cec230992f07a8a281d6a2ba067",
        "Teenage Mutant Hero Turtles II - Back from the Sewers",
        "(Europe)",
        "DMG-TTX",
        3475263596,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "58caba989564d33889b700e596b4a75c5cc8bfe3",
        "Spud's Adventure",
        "(USA)",
        "DMG-TVE",
        1894116761,
        65536,
        "En",
        "USA"
   , " " },
    {
        "4c070d01b3002008e6ac35d12e8397e6536ef86c",
        "Totsugeki Valations",
        "(Japan)",
        "DMG-TVJ",
        4061239853,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "4a6816d30d31831341503680eb4de5fd057767cd",
        "Twin",
        "(Japan)",
        "DMG-TWJ",
        605269162,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "b537a2cfd3192094580aeb31fe81db1ace755bda",
        "Tiny Toon Adventures - Babs' Big Break",
        "(USA, Europe)",
        "DMG-TXE",
        2606653977,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "e75112788dd730a38230a73708ac8099ec16d8c2",
        "Tiny Toon Adventures",
        "(Japan)",
        "DMG-TXJ",
        4212344363,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "390c1af89684e1fd02e62b52fcadd8a416b190ef",
        "Taiyou no Yuusha - Fighbird GB",
        "(Japan)",
        "DMG-TYJ",
        1916713090,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "c692d3cb4a2939eebc534b963515f95bedf837cc",
        "Terminator 2 - Judgment Day",
        "(USA, Europe)",
        "DMG-TZE",
        562331905,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "5bb102624d756ade9aa26c4f9745a4c0a9502655",
        "Ultima - Runes of Virtue II",
        "(USA)",
        "DMG-U2E",
        3293182911,
        262144,
        "En",
        "USA"
   , " " },
    {
        "9af7e3f05e1055591e94ed10876cfd9bd707952d",
        "Ultima - Ushinawareta Runes 2",
        "(Japan)",
        "DMG-U2J",
        2037372581,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "81cca20c6d1505ea917a714820bd75c6c3b58d6c",
        "America Oudan Ultra Quiz Part 3 - Champion Taikai",
        "(Japan)",
        "DMG-U3J",
        2160771964,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "823d37e2028eb2875e116f16c8553ba187b43962",
        "3-pun Yosou - Umaban Club",
        "(Japan)",
        "DMG-U4J",
        3405647147,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "465947e8f365b29c1a11c0f5330e6c3a746867b5",
        "Sports Illustrated for Kids - The Ultimate Triple Dare!",
        "(USA)",
        "DMG-U8E",
        4040650057,
        262144,
        "En",
        "USA"
   , " " },
    {
        "4d7a8a5c6615ca5755404817fdea9a8a96e9b45e",
        "America Oudan Ultra Quiz Part 4",
        "(Japan)",
        "DMG-UAJ",
        2808103880,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "26f6067d7c4569617a85445edc61ad932c67cf40",
        "Blades of Steel",
        "(USA)",
        "DMG-UBE",
        3894190009,
        131072,
        "En",
        "USA"
   , " " },
    {
        "ff48fe9a931f94e35ee9d1a32be4e61f7d7dbe1e",
        "Konamic Ice Hockey",
        "(Japan)",
        "DMG-UBJ",
        3511545351,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "8033eee55ac765878034effb3f5e3da80185ff65",
        "Blades of Steel",
        "(Europe)",
        "DMG-UBX",
        4156424194,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "a9040d4d50682ae91694a9d95aead23c9275365e",
        "Uno - Small World",
        "(Japan)",
        "DMG-UCJ",
        2742813229,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "3e6de1d028b4f3970ce192cc687f7a933b6cf49b",
        "Universal Soldier",
        "(USA, Europe)",
        "DMG-UDE",
        2578357510,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "4aab3bb1072b6f16fa5bf859aaaf730b2465bdab",
        "Mr. Go no Baken Tekichuu Jutsu",
        "(Japan)",
        "DMG-UEJ",
        3940217319,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "147-25565",
        "Smurfs, The",
        "(Europe) (En,Fr,De,Es)",
        "DMG-UFX",
        3456046623,
        131072,
        "En,Fr,De,Es",
        "Europe",
        "/the_smurfs64.sprite"
    },
    {
        "101-43484",
        "Smurfs, The",
        "(USA, Europe) (En,Fr,De) (Rev 1) (SGB)",
        "DMG-UFX",
        2338049511,
        131072,
        "En,Fr,De",
        "USA, Europe",
        "/the_smurfs64.sprite"
    },
    {
        "021db2fa5df10677e414aaf843c8363e59cddb45",
        "Yu Yu Hakusho Dai-3-dan - Makai no Tobira Hen",
        "(Japan)",
        "DMG-UGJ",
        633739510,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "61f61c29b68bcde06a24c14f0bd17af099cbeaa3",
        "Prehistorik Man",
        "(USA, Europe)",
        "DMG-UHE",
        3307225430,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "07efa685cf17bd1a2d53e337f72d9f1328846401",
        "P-Man GB",
        "(Japan)",
        "DMG-UHJ",
        1920816393,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "1bf1bc4206c80623ca3fdba77ed43cf6232c8120",
        "Saigo no Nindou",
        "(Japan)",
        "DMG-UJA",
        3093603254,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "5719fff28260ec6868c813b6d8a6bbabd58afaca",
        "Yu Yu Hakusho Dai-2-dan - Ankoku Bujutsukai Hen",
        "(Japan)",
        "DMG-UKJ",
        1785557840,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "29857c7539c7cb1b059a6b9ecd8e8a61c35c2f28",
        "Ultraman Club - Tekikaijuu o Hakken seyo!",
        "(Japan)",
        "DMG-ULJ",
        2448271456,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "528f2c6c18d0c45a264203da038f22163d325a24",
        "Minesweeper - Soukaitei",
        "(Japan)",
        "DMG-UMA",
        1429386193,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "79877af1002e97a53b79dd49377fc6f5db03bf53",
        "Ultraman",
        "(Japan)",
        "DMG-UNJ",
        1497678829,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "b586fd39813be5c5302b75015a4c4d4a68e205cc",
        "Shin SD Gundam Gaiden - Knight Gundam Monogatari",
        "(Japan) (SGB)",
        "DMG-UOJ",
        3852962715,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "3118006627237c82bfd9d4bed4328e1ac034fd06",
        "Ninja Taro",
        "(USA)",
        "DMG-UPE",
        3309223869,
        131072,
        "En",
        "USA"
   , " " },
    {
        "a85bd7dc735fba0e9370819785850c52f3fa56be",
        "Sengoku Ninja-kun",
        "(Japan)",
        "DMG-UPJ",
        1806580289,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "f5aa60d80828b7f4f68fbd8c0ab86ac6cab3b246",
        "America Oudan Ultra Quiz",
        "(Japan)",
        "DMG-UQJ",
        1004697037,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "ef8790596c4948b93f46a71a49c4a062d3a856b7",
        "Turn and Burn - The F-14 Dogfight Simulator",
        "(USA)",
        "DMG-URE",
        2436451288,
        131072,
        "En",
        "USA"
   , " " },
    {
        "cf0dede6615ff25cae8222be86a6c2a830d1deb5",
        "Phantom Air Mission",
        "(Europe)",
        "DMG-URX",
        2328810662,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "65aaadfe1cdd28eb157de5fcc74091d8cb4ba3d9",
        "Fish Dude",
        "(USA)",
        "DMG-USE",
        1267898137,
        65536,
        "En",
        "USA"
   , " " },
    {
        "3657748a92ae7396eb23c7df7003fbd337fbf40b",
        "Uoozu",
        "(Japan)",
        "DMG-USJ",
        4158769605,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "61b4835b6fae7cae5e3ae25c485718f263221216",
        "Ultima - Runes of Virtue",
        "(USA)",
        "DMG-UTE",
        3293187870,
        131072,
        "En",
        "USA"
   , " " },
    {
        "a3fe79cfb3c65835c52661e676450d3d738660b4",
        "Ultima - Ushinawareta Runes",
        "(Japan)",
        "DMG-UTJ",
        3539550593,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "1fd844c91be228b6709513517ec6082895e78c08",
        "Yu Yu Hakusho",
        "(Japan)",
        "DMG-UUJ",
        1896068268,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "80eb68081e7bb437c4728471d8bb3b16aa97f0b1",
        "Mickey's Ultimate Challenge",
        "(USA, Europe)",
        "DMG-UVE",
        602527269,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "e2e384548675887ecfe31561c3f4f6bc6ed83764",
        "Solitaire FunPak",
        "(USA, Europe)",
        "DMG-UXE",
        900014922,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "acb5b79fd3c60071e22b946cd212b7464c77f581",
        "Urusei Yatsura - Miss Tomobiki o Sagase!",
        "(Japan)",
        "DMG-UYJ",
        1451232363,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "2a094e724f77f688e8e1d5e3fe0a5437a96eb4b7",
        "Undercover Cops Gaiden - Hakaishin Garumaa",
        "(Japan)",
        "DMG-UZJ",
        3081713653,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "4959bbc7022492481ab10b24d4a7a7ef68ce8970",
        "Flappy Special",
        "(Japan)",
        "DMG-V2A",
        996990372,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "2f1a336b01983acae04b879f316c4b97ccbcc080",
        "Top Gear Pocket 2",
        "(Japan) (Rumble)",
        "CGB-V33J",
        407237210,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "256e993a9cf931b045b32ffcf7596f0a8de90741",
        "Mini-Putt",
        "(Japan)",
        "DMG-V3A",
        869102928,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "62295f6fa651e1d0b91e522b9d79bf5f39e4843a",
        "Choplifter II",
        "(Japan)",
        "DMG-V4A",
        1359606916,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "1fb52a3373cf6051abc76049518beb06702a5a13",
        "Choplifter II - Rescue & Survive",
        "(USA)",
        "DMG-V4E",
        2625461881,
        131072,
        "En",
        "USA"
   , " " },
    {
        "1f05ebe4888ccc1867e34ec4c983eca0381e2a37",
        "Choplifter II - Rescue & Survive",
        "(Europe)",
        "DMG-V4X",
        1580092953,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "73a311f205168e746f531e19b05f2981864d299c",
        "Nontan to Issho - Kurukuru Puzzle",
        "(Japan)",
        "DMG-V5J",
        1825979147,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "b5272d442552ca1cf397b57d37aded31aa05add6",
        "Super Street Basketball 2",
        "(Japan) (SGB)",
        "DMG-VBA",
        2156972492,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "2952f3ed3146b5881122e9bf1f9db6c38d6c575d",
        "Chee-Chai Alien",
        "(Japan) (Rumble)",
        "CGB-VCAJ",
        2660798462,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "fa3e16ae028f4c117011aa87dfd50d8b0d50ce3c",
        "Prophecy - The Viking Child",
        "(USA)",
        "DMG-VCE",
        2163617690,
        262144,
        "En",
        "USA"
   , " " },
    {
        "6e64f9f0ab6013dd0ea5867b4bc2bcc544f11b92",
        "Prophecy - The Viking Child",
        "(Europe) (En,Fr,De,Es,It)",
        "DMG-VCX",
        1494179761,
        262144,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "c2042bdf7d87adb273831c6c8e9c6a6a561cec0a",
        "Ren & Stimpy Show, The - Veediots!",
        "(USA, Europe)",
        "DMG-VDE",
        1347206429,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "ea90791b7818d9af8ed806ea96e79ba1bc3ca8b8",
        "Rolan's Curse",
        "(USA)",
        "DMG-VEE",
        442508688,
        65536,
        "En",
        "USA"
   , " " },
    {
        "d84e92a748cc4f0f0820c4c5a9ac0c993ddd7cf1",
        "Velious - Roland no Majuu",
        "(Japan)",
        "DMG-VEJ",
        135768626,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "fafd80483db456dfb0cdc583503cffabcf30b8e7",
        "Volley Fire",
        "(Japan)",
        "DMG-VFJ",
        218556978,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "4aa354120bcac6fec35496816318108e101db649",
        "Vattle Giuce",
        "(Japan)",
        "DMG-VGA",
        2506842172,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "9325a4e58aedb0a69e6326afeb28b284157eb819",
        "Top Gear Pocket",
        "(USA) (Rumble)",
        "CGB-VGRE",
        2219417537,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "00349bc7be442a24c90cd86da9e803f321cc9f95",
        "Top Gear Pocket",
        "(Japan) (En) (Rumble)",
        "CGB-VGRJ",
        3956057050,
        1048576,
        "En",
        "Japan"
   , " " },
    {
        "af807873ddc048601e6fa50ebc7b12cd5dd8b7f5",
        "Top Gear Rally",
        "(Europe) (Rumble)",
        "CGB-VGRP",
        863919571,
        1048576,
        "En",
        "Europe"
   , " " },
    {
        "f85b5c81659c0afd7db4c53d5273f6a49e3d8e36",
        "Hole in One Golf",
        "(USA) (Rumble) (SGB) (GB Comp.)",
        "DMG-VHIE",
        665139557,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "e7c37298311fb263c0d9eba933151faf3efe978e",
        "Versus Hero - Kakutou Ou e no Michi",
        "(Japan)",
        "DMG-VHJ",
        1151136484,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "ff00396361f06e5d2d4843e2e0b55479c7026f36",
        "Kiteretsu Daihyakka - Bouken Ooedo Juraki",
        "(Japan)",
        "DMG-VIJ",
        3577039024,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "df60c388deffa4baeb644de7631c5339414746fd",
        "Shinri Game, The",
        "(Japan)",
        "DMG-VKJ",
        4214219618,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "f5931faf23072e57e8c3190229dfda19cd656164",
        "Missile Command",
        "(USA) (Rumble)",
        "CGB-VLCE",
        1196702801,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "2f036efd986a70dafef47e537b6a29763af19b06",
        "Little Mermaid II, The - Pinball Frenzy",
        "(USA) (En,Fr,De,Es,It) (Rumble)",
        "CGB-VM2E",
        911187149,
        1048576,
        "En,Fr,De,Es,It",
        "USA"
   , " " },
    {
        "e5d2e5bcfa3f1f9b448082ff5d736b02ee751eac",
        "MVP Baseball",
        "(Japan)",
        "DMG-VMA",
        952182442,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "7e6d1979888cd6f750c09c03c390dc263f1376fd",
        "Roger Clemens' MVP Baseball",
        "(USA) (Rev 1)",
        "DMG-VME",
        4204092217,
        262144,
        "En",
        "USA"
   , " " },
    {
        "9402685c92b2ec18fa24cb2d589807e928df5bbd",
        "Roger Clemens' MVP Baseball",
        "(USA)",
        "DMG-VME",
        2645055835,
        262144,
        "En",
        "USA"
   , " " },
    {
        "f9c7ba41df58fdee0b9a6e869382d362e3e98929",
        "NASCAR Challenge",
        "(USA) (Rumble)",
        "CGB-VNCE",
        969324648,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "c90303c433c42985155c60cddcbcd3982a82b186",
        "Battletoads-Double Dragon",
        "(USA)",
        "DMG-VNE",
        2804414925,
        262144,
        "En",
        "USA"
   , " " },
    {
        "e230d9ae0a2cd472ae90aa677f368b214da0a600",
        "Battletoads-Double Dragon",
        "(Europe)",
        "DMG-VNX",
        2065789058,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "105-29154",
        "10-Pin Bowling",
        "(USA) (Rumble)",
        "CGB-VP9E",
        1913417763,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "fb3a7f68282428a10befeaea58e2b83ecae0e188",
        "Perfect Dark",
        "(USA, Europe) (En,Fr,De,Es,It) (Rumble)",
        "CGB-VPDE",
        100777718,
        4194304,
        "En,Fr,De,Es,It",
        "USA, Europe"
   , " " },
    {
        "f47322558f3a6f2210aa615f6f49d046e7cb6859",
        "Captain America and the Avengers",
        "(USA)",
        "DMG-VPE",
        3345135491,
        131072,
        "En",
        "USA"
   , " " },
    {
        "4b6edd6fb0dd90ba1e2c78a83c9127ff63e66e78",
        "Super Real Fishing",
        "(Japan) (Rumble)",
        "CGB-VPFJ",
        8802657,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "70-55904",
        "Pokemon Pinball",
        "(USA, Australia) (Rumble) (SGB) (GB Comp.)",
        "DMG-VPHE",
        63868314,
        1048576,
        "En",
        "USA, Australia",
        "/pokemon_pinball64.sprite" 
    },
    {
        "66-55216",
        "Pokemon Pinball",
        "(Japan) (Rumble) (SGB) (GB Comp.)",
        "DMG-VPHJ",
        331812329,
        1048576,
        "Ja",
        "Japan",
        "/pokemon_pinball64.sprite" 
    },
    {
        "41-6689",
        "Pokemon Pinball",
        "(Europe) (En,Fr,De,Es,It) (Rumble) (SGB) (GB Comp.)",
        "DMG-VPHP",
        969159332,
        2097152,
        "En,Fr,De,Es,It",
        "Europe",
        "/pokemon_pinball64.sprite" 
    },
    {
        "73923a7e31cd14ee00705cb64fa2bae88b909384",
        "Polaris SnoCross",
        "(USA) (Rumble)",
        "CGB-VPSE",
        3716880542,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "8f3a37e6e9642b1368a25ceea3f46367a8c66837",
        "Test Drive Off-Road 3",
        "(USA) (Rumble) (SGB) (GB Comp.)",
        "DMG-VQ3E",
        266165150,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "157f5d98697e84723042654ceaf40a9ec3ab89dd",
        "RoboCop versus The Terminator",
        "(USA)",
        "DMG-VRE",
        4163727907,
        131072,
        "En",
        "USA"
   , " " },
    {
        "5471c52478aff1c3febe1bb0f51494504d89a383",
        "Super Black Bass - Real Fight",
        "(Japan) (Rumble)",
        "CGB-VRFJ",
        3086449514,
        4194304,
        "Ja",
        "Japan"
   , " " },
    {
        "46d8299a5423d14e31e7610234f087ad05770d57",
        "Get Chuu Club - Minna no Konchuu Daizukan",
        "(Japan) (Rumble) (SGB) (GB Comp.)",
        "DMG-VRKJ",
        2663233222,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "312e43188731908af6f07bbdc952be981c6d1c0a",
        "Ready 2 Rumble Boxing",
        "(USA) (Rumble)",
        "CGB-VRRE",
        878583972,
        2097152,
        "En",
        "USA"
   , " " },
    {
        "1a6fce6039e15346bebce32abbf36c59b1e19fb2",
        "RoboCop versus The Terminator",
        "(Europe)",
        "DMG-VRX",
        2654668228,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "d885ffa933b89b69b1fda9e21ebb527ce529115f",
        "VS Battler",
        "(Japan)",
        "DMG-VSA",
        548288666,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "8a4409b3c7b3ceca6452527c6930dbca3a84604a",
        "Bionic Battler",
        "(USA)",
        "DMG-VSE",
        2716163522,
        65536,
        "En",
        "USA"
   , " " },
    {
        "f5003c7d05cf3f4853f573da6c264d5c948533ef",
        "Rolan's Curse II",
        "(USA)",
        "DMG-VTE",
        659878752,
        131072,
        "En",
        "USA"
   , " " },
    {
        "bc9a1bf8a71bd46d7de80bc4bf5d44846cd61d57",
        "Velious II - Fukushuu no Jashin",
        "(Japan)",
        "DMG-VTJ",
        3042882599,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "11b7218f1f23870c77551895a170c66e2e470e31",
        "Tonka Raceway",
        "(USA) (Rumble)",
        "CGB-VTQE",
        2779728680,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "0be8c0a2ed48ff5c8059dae27b83b29ac318929b",
        "Dr. Mario",
        "(World)",
        "DMG-VUA",
        281644497,
        32768,
        "En",
        "World"
   , " " },
    {
        "48b2eb34d01f0f47885f0cfac5ba9676509beffe",
        "Dr. Mario",
        "(World) (Rev 1)",
        "DMG-VUA",
        4028784080,
        32768,
        "En",
        "World"
   , " " },
    {
        "0-17890",
        "3-D Ultra Pinball - Thrillride",
        "(USA) (Rumble)",
        "CGB-VUPE",
        392619352,
        524288,
        "En",
        "USA"
   , " " },
    {
        "6841e073fb72fb18bc984690a16999b3e178af72",
        "Kawa no Nushi Tsuri 4",
        "(Japan) (Rumble) (SGB) (GB Comp.)",
        "DMG-VUTJ",
        4038106587,
        1048576,
        "Ja",
        "Japan"
   , " " },
    {
        "04d1e345eba87d535e07699b257d4e225824f548",
        "Vigilante 8",
        "(USA) (Rumble)",
        "CGB-VV8E",
        3517024476,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "3b0022783904aa6921ea671bf73d3d4acf1218a6",
        "Bakenou V3",
        "(Japan)",
        "DMG-VVJ",
        1311768549,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "0049eda7f9c13016eeff5da0104d3982cd8d39df",
        "Nushi Tsuri Adventure - Kite no Bouken",
        "(Japan) (Rumble)",
        "CGB-VVJJ",
        2891118319,
        2097152,
        "Ja",
        "Japan"
   , " " },
    {
        "ef1ce2865d7cc663e447e7ec8ce78f6c445965e5",
        "Nekketsu! Beach Volley Da yo Kunio-kun",
        "(Japan) (SGB)",
        "DMG-VXJ",
        2885387487,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "6-40480",
        "Star Wars Episode I - Racer",
        "(USA, Europe) (Rumble)",
        "CGB-VYHE",
        247224152,
        2097152,
        "En",
        "USA, Europe",
        "/SWI_racer64.sprite" },
    {
        "750a2e02eb19b6433a0ae7871edec1605fc6d5ca",
        "Zebco Fishing!",
        "(USA) (Rumble)",
        "CGB-VZFE",
        1019654649,
        1048576,
        "En",
        "USA"
   , " " },
    {
        "ff76385fbff29f3bdf90ae7c329c0e16a106ded5",
        "Sensible Soccer - European Champions",
        "(Europe)",
        "DMG-VZX",
        3171034712,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "e4a002e304b0c86c057c45fec922fdf04db0075a",
        "Mega Man II",
        "(USA)",
        "DMG-W2E",
        3835098758,
        262144,
        "En",
        "USA"
   , " " },
    {
        "c2ac885f4e547c1bdfebed8ed67248755985c605",
        "Rockman World 2",
        "(Japan)",
        "DMG-W2J",
        3276613214,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "4cb3f4a2ae47c9d1862babb513bec69d595d9c11",
        "Mega Man II",
        "(Europe)",
        "DMG-W2X",
        1586547528,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "c5664b8c63e3aadedb3509d2cf0345931cf9f509",
        "Mega Man III",
        "(USA)",
        "DMG-W3E",
        1366677345,
        262144,
        "En",
        "USA"
   , " " },
    {
        "17c608dbcdf391829faf32a6c5b8d286ae6503a7",
        "Rockman World 3",
        "(Japan)",
        "DMG-W3J",
        3909371983,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "601ceff80488b177a21784c09320e740f9b4f761",
        "Mega Man III",
        "(Europe)",
        "DMG-W3X",
        61920492,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "77858f96fb33a867d0edb3e7760ea4944f4ef99b",
        "Wizardry Gaiden II - Kodai Koutei no Noroi",
        "(Japan)",
        "DMG-W5J",
        4135742738,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "0c98d979b6cb6993f775bcfc2e386c4caa220f3b",
        "Wizardry Gaiden III - Yami no Seiten",
        "(Japan)",
        "DMG-W6J",
        852051256,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "3a897ea656354d8f7308c9b6262cbcc9d2886f10",
        "J.League Winning Goal",
        "(Japan)",
        "DMG-W7J",
        2914283420,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "4e1135b6e94db954eec2009096cc12a862473a47",
        "WCW - The Main Event",
        "(USA, Europe)",
        "DMG-W8E",
        2538500395,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "488da3fe31cec951076632c3f09d0cab6622f6cf",
        "World Cup USA 94",
        "(Japan)\n (En,Fr,De,Es,It,Nl,Pt,Sv)",
        "DMG-W9A",
        2636642782,
        262144,
        "En,Fr,De,Es,It,Nl,Pt,Sv",
        "Japan"
   , " " },
    {
        "8236332cc5224b7c3dbbd1554fd32e0a1416d125",
        "World Cup USA 94",
        "(Europe)\n (En,Fr,De,Es,It,Nl,Pt,Sv)",
        "DMG-W9E",
        1612914563,
        262144,
        "En,Fr,De,Es,It,Nl,Pt,Sv",
        "Europe"
   , " " },
    {
        "f50e64611b61e196b372c7274fa3184958e73f4d",
        "Wave Race",
        "(USA, Europe)",
        "DMG-WAE",
        2790872326,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "48db2cabd93ccb1712d966b21d17d78a7e7ba529",
        "World Bowling",
        "(Japan)",
        "DMG-WBA",
        1207873010,
        32768,
        "Ja",
        "Japan"
   , " " },
    {
        "36d1b64565215c9f421d90bdc8fff58ba5abf044",
        "World Bowling",
        "(USA)",
        "DMG-WBE",
        2305717922,
        32768,
        "En",
        "USA"
   , " " },
    {
        "c032e1a823c3ea556741b5a62524a6873e7a4693",
        "Nigel Mansell's World Championship Racing",
        "(USA)",
        "DMG-WCE",
        65555837,
        131072,
        "En",
        "USA"
   , " " },
    {
        "82ed9fb7ea610bf905102b2f9843b9fd07152021",
        "Nigel Mansell's World Championship",
        "(Europe)",
        "DMG-WCX",
        3786539739,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "233d833f640a6490b46c8eb990a072f6cecfdd4b",
        "Nigel Mansell's World Championship Racing",
        "(Europe)",
        "DMG-WCY",
        218716323,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "c4588e7f3d5df19c37969be0809a26831656a8bc",
        "Winner's Horse",
        "(Japan)",
        "DMG-WEJ",
        350820183,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "08696d3ed7c06da77448f02e91263e3cda235c96",
        "Gluecksrad",
        "(Germany)",
        "DMG-WFD",
        3904005495,
        131072,
        "De",
        "Germany"
   , " " },
    {
        "8aeb6f2d82169a94269096fb9ca5be4bcfed3a23",
        "Wheel of Fortune",
        "(USA)",
        "DMG-WFE",
        2215181896,
        131072,
        "En",
        "USA"
   , " " },
    {
        "ebdaf82e97fd14c707600e7fc4c5e045f40da988",
        "Swamp Thing",
        "(USA, Europe)",
        "DMG-WGE",
        1991140040,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "836ed2ac4069065939ae33d72edecd7ac44faec6",
        "World Ice Hockey",
        "(Japan)",
        "DMG-WHA",
        1312051235,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "483a58ecc72660a017cff3cee61c3f284c74980b",
        "Wizardry Gaiden I - Joou no Junan",
        "(Japan)",
        "DMG-WIJ",
        1298402931,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "159203803d17c4abdd8c348fd61bf996885241d6",
        "Wario Land - Super Mario Land 3",
        "(World)",
        "DMG-WJA",
        1086208137,
        524288,
        "En",
        "World"
   , " " },
    {
        "eadbba2a7f403324a62ec79b59f1ce3513f33a9b",
        "StarHawk",
        "(Europe)",
        "DMG-WKE",
        3761431810,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "0b7750446b094f24ea9c05100fb820febf18e8c1",
        "Cool World",
        "(USA, Europe)",
        "DMG-WLE",
        2710814928,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "3d238e63b173c339508ca38156708b392306cd40",
        "Mega Man V",
        "(USA) (SGB)",
        "DMG-WME",
        1927729693,
        524288,
        "En",
        "USA"
   , " " },
    {
        "58ec5f302422fd5e1d8ec93b85e695fe00a5e764",
        "Rockman World 5",
        "(Japan) (SGB)",
        "DMG-WMJ",
        4004238278,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "756cdbd3ba262b9dff5040a0f65e8c15cf9dfce5",
        "Mega Man V",
        "(Europe) (SGB)",
        "DMG-WMX",
        3509241086,
        524288,
        "En",
        "Europe"
   , " " },
    {
        "863b62d6b3f414df0fca0821806b293a13fbc511",
        "Wayne's World",
        "(USA)",
        "DMG-WNE",
        2191434789,
        262144,
        "En",
        "USA"
   , " " },
    {
        "ea3c26dee06072555124d32c7a1e3e96c7ffab87",
        "Pingu - Sekai de 1ban Genki na Penguin",
        "(Japan)",
        "DMG-WOJ",
        2833088710,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "6af3ae662c598f946e02ed796bdd643c8717c000",
        "WWF King of the Ring",
        "(Japan)",
        "DMG-WPA",
        984962579,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "7a0481a3dcd0a426f0903bf7c1418d8d9fbf8460",
        "WWF King of the Ring",
        "(USA, Europe)",
        "DMG-WPE",
        2131548284,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "204-1119",
        "3 Choume no Tama - Tama and Friends - 3 Choume Obake Panic!!",
        "(Japan)",
        "DMG-WQJ",
        3055341856,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "3e11b19414d08ef0370de3b56cd121711aabe5d3",
        "Bakuretsu Senshi Warrior",
        "(Japan)",
        "DMG-WRA",
        3975264257,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "ecff5c822b89285a04307207681c57f3a6b7bc7c",
        "Cyraid",
        "(USA)",
        "DMG-WRE",
        2634078805,
        65536,
        "En",
        "USA"
   , " " },
    {
        "5d427212813c1df8915206fc032fbc5f974b47c5",
        "Star Wars",
        "(USA)",
        "DMG-WSE",
        2954731950,
        131072,
        "En",
        "USA"
   , " " },
    {
        "00ce176e182472a4f9ad39876037480141b9ea72",
        "Star Wars",
        "(Europe)",
        "DMG-WSX",
        2118085566,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "3664dc1f2852f7ee6f5e210d3f9bbdecc8d429ea",
        "Star Wars",
        "(USA, Europe) (Rev 1)",
        "DMG-WSX",
        1569581915,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "f5153bc730119f8c3db42fba6a72019caa6a08cc",
        "Wordtris",
        "(USA)",
        "DMG-WTE",
        123372081,
        131072,
        "En",
        "USA"
   , " " },
    {
        "e09001ead7d882a64f9254d91150fb43e432b785",
        "World Beach Volley - 1991 GB Cup",
        "(Japan)",
        "DMG-WVA",
        2726905217,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "176966281aa2f04f928c40f639f779620970e77a",
        "World Beach Volley - 1992 GB Cup",
        "(Europe)",
        "DMG-WVE",
        1076093061,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "6dc4917350a27deabe79246c01425f70684a41c5",
        "Wizards & Warriors X - The Fortress of Fear",
        "(USA, Europe)",
        "DMG-WWE",
        273593603,
        65536,
        "En",
        "USA, Europe"
   , " " },
    {
        "4a58ff926d9b18c1bcf3f09c7c8bcc94ceaed29d",
        "WWF Superstars 2",
        "(Japan)",
        "DMG-WXA",
        3390232402,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "a233b6e89a91124ca54f9abf3092e4a34eb6a147",
        "WWF Superstars 2",
        "(USA, Europe)",
        "DMG-WXE",
        2395872260,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "eb4dac6e49272179faf7d6476eceb2f1660b0e84",
        "WordZap",
        "(USA)",
        "DMG-WZE",
        3907353424,
        65536,
        "En",
        "USA"
   , " " },
    {
        "19cdd237624c4e8b7f680ab817524cae7ab2d2e4",
        "Xenon 2 - Megablast",
        "(Japan)",
        "DMG-X2A",
        803958994,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "9c24e8798895eb7671df7bb635289a36af8978c9",
        "Xenon 2",
        "(USA, Europe)",
        "DMG-X2E",
        3728311928,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "21f30f90d180e9db661371fc26806841ce5fcca8",
        "Nettou Garou Densetsu 2 - Aratanaru Tatakai",
        "(Japan) (SGB)",
        "DMG-X3J",
        2862114943,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "7b7013b555d8b856e7dc6d7e69a96bdc50868e8a",
        "Samurai Shodown",
        "(USA, Europe) (SGB)",
        "DMG-X4E",
        1764306662,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "977216e15dd3252a8e48e60ebec358d384c5c7d1",
        "Nettou Samurai Spirits",
        "(Japan) (Rev 1) (SGB)",
        "DMG-X4J",
        3376959692,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "d154af2add1498389761055e521c1ed657671757",
        "Nettou Samurai Spirits",
        "(Japan) (SGB)",
        "DMG-X4J",
        1244684918,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "7ee8c431f899dab4a7ab6d80d08b9c76b304ceef",
        "Monster Max",
        "(Europe) (En,Fr,De,Es,It,Nl)",
        "DMG-X8E",
        806034733,
        262144,
        "En,Fr,De,Es,It,Nl",
        "Europe"
   , " " },
    {
        "4331240ad4b9dc71b3ce47116f571ace8b63d99e",
        "Asterix",
        "(Europe) (En,Fr,De,Es,It)",
        "DMG-XAX",
        159383084,
        131072,
        "En,Fr,De,Es,It",
        "Europe"
   , " " },
    {
        "fe2fbbaf92d7b512897f36e0b1d1936aea5147d9",
        "Muhammad Ali Heavyweight Boxing",
        "(USA, Europe)",
        "DMG-XBE",
        2545761935,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "39b144321648f807812fd4e065dc04d655c4722b",
        "Power Mission",
        "(Japan)",
        "DMG-XCA",
        237638676,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "ef76439bfa42ee9ed77a710d64d7451e23b83177",
        "Power Mission",
        "(Japan) (Rev 1)",
        "DMG-XCA",
        3809409195,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "331748458d13071ac648cc31c9c15bda3a2840e2",
        "Power Mission",
        "(USA)",
        "DMG-XCE",
        1187721011,
        131072,
        "En",
        "USA"
   , " " },
    {
        "674f9421456f662f6b007a5211c22d0fe608f4cf",
        "Zerd no Densetsu 2 - Xerd!! Gishin no Ryouiki",
        "(Japan)",
        "DMG-XDJ",
        2711076279,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "53bd3b9892301e5278bbbcc8d09cef85e42729e1",
        "Spider-Man - X-Men - Arcade's Revenge",
        "(USA, Europe)",
        "DMG-XGE",
        269076160,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "455d2bb27300c8e740d2fc85d4faad335195fc18",
        "Toxic Crusaders",
        "(USA)",
        "DMG-XLE",
        3931752163,
        131072,
        "En",
        "USA"
   , " " },
    {
        "a53da9526cf0d825d121931611ead731364d5441",
        "Batman - The Animated Series",
        "(USA, Europe)",
        "DMG-XME",
        3370481855,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "83946e77896b1b54db5e9109f1ddebf6e4b8967d",
        "Ferrari",
        "(Japan)",
        "DMG-XPA",
        2814246600,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "d1dc8f6de250a6a453118665fba60db6ada8aa59",
        "Ferrari Grand Prix Challenge",
        "(USA, Europe)",
        "DMG-XPE",
        3982979547,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "dbe5dc96430bec521c35d8b884de1cc1863735d7",
        "SD Lupin Sansei - Kinko Yaburi Daisakusen",
        "(Japan)",
        "DMG-XRJ",
        4038540724,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "61a6bd9fbcf48f9400a64cf0971670463134ad58",
        "Top Rank Tennis",
        "(USA)",
        "DMG-XTE",
        1011755444,
        262144,
        "En",
        "USA"
   , " " },
    {
        "95021ce0e656af746b3e18fd0b1d0cad98ddec58",
        "Top Ranking Tennis",
        "(Europe)",
        "DMG-XTX",
        3581401312,
        262144,
        "En",
        "Europe"
   , " " },
    {
        "36dce385bf0231382805547bbd21dc3485b7ee2c",
        "Double Yakuman Jr.",
        "(Japan)",
        "DMG-XVJ",
        883716712,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "20f4fa230a1a15137c5bfa37533b1fd4b836f064",
        "Riddick Bowe Boxing",
        "(USA)",
        "DMG-XWE",
        4217185504,
        131072,
        "En",
        "USA"
   , " " },
    {
        "16b44ecd73294e064b08f5dfe81c166230ca3034",
        "Riddick Bowe Boxing",
        "(Europe)",
        "DMG-XWX",
        2380377756,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "4b6530b3512e1f489cf37c9df2983a0f15043b07",
        "Paperboy 2",
        "(USA, Europe)",
        "DMG-Y2E",
        2397308106,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "35f0e22fbc314f2ffe5596b33984843de317b383",
        "Pierre le Chef is... Out to Lunch",
        "(Europe)",
        "DMG-Y5X",
        21347180,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "2e618f62cf19b6c37db404f8f5ec0fb17cb86fe4",
        "Uchuu Senkan Yamato",
        "(Japan)",
        "DMG-YAJ",
        1542932472,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "39f786b5959af538d947f39f11e0e1dfa2c3106e",
        "Panel Action Bingo",
        "(USA)",
        "DMG-YBE",
        3489702166,
        65536,
        "En",
        "USA"
   , " " },
    {
        "72b32d7085f7148acdbebb1953b08b957e1a7469",
        "Winter Olympic Games",
        "(USA)",
        "DMG-YCE",
        446230692,
        131072,
        "En",
        "USA"
   , " " },
    {
        "f7462e60108d0e67c1ffab6f2cdc868541478eff",
        "Winter Gold",
        "(Europe)",
        "DMG-YCX",
        1011667525,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "f3d1df77e96209cf57720246bc2fbfee77bf209b",
        "Dennis the Menace",
        "(USA)",
        "DMG-YDE",
        2125516082,
        131072,
        "En",
        "USA"
   , " " },
    {
        "f059b485fc682a63ec64d6d3f0fa7da7168531d6",
        "Dennis",
        "(Europe)",
        "DMG-YDX",
        2305437864,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "e0a6c2a743af42b047e17716e71ad524e10ede82",
        "In Your Face",
        "(USA)",
        "DMG-YFE",
        2158774398,
        131072,
        "En",
        "USA"
   , " " },
    {
        "4b6527e4060e12f3ac19686a093346be693157aa",
        "Yogi Bear's Gold Rush",
        "(USA)",
        "DMG-YGE",
        2825639963,
        131072,
        "En",
        "USA"
   , " " },
    {
        "111833b017025f92e855b7467684ce0d2fa87534",
        "Yogi Bear's Gold Rush",
        "(Europe)",
        "DMG-YGX",
        1853600822,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "07a4af453b7352c1a4e322e4517ba65419ce4236",
        "Simpsons, The - Itchy & Scratchy - Miniature Golf Madness",
        "(USA, Europe)",
        "DMG-YHE",
        3719383013,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "43a8273505e7e4298bdc4068e4b207b5a067bb7d",
        "World Cup Striker",
        "(Japan)",
        "DMG-YKA",
        4260341901,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "cc11003a61d8bf3a6199f55dded248dfd1c343f0",
        "Elite Soccer",
        "(USA) (SGB)",
        "DMG-YKE",
        4114700454,
        131072,
        "En",
        "USA"
   , " " },
    {
        "141-48645",
        "Soccer",
        "(Europe, Australia) (En,Fr,De) (SGB)",
        "DMG-YKY",
        3278576027,
        131072,
        "En,Fr,De",
        "Europe, Australia"
   , " " },
    {
        "7332228a47e208b63b301bfb7adb8700a0d156de",
        "Ganso!! Yancha Maru",
        "(Japan)",
        "DMG-YMJ",
        4473830,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "8f34cc6313bf0f17f794fe9592a60ef95df89b57",
        "Tsumeshougi - Mondai Teikyou Shougi Sekai",
        "(Japan)",
        "DMG-YNJ",
        1677963014,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "1536c9111624511dec76222ff4ad490c9354fc1e",
        "Yoshi",
        "(USA)",
        "DMG-YOE",
        4076125386,
        65536,
        "En",
        "USA"
   , " " },
    {
        "67bbc822f58f7042b74a7328d18f5fe9fd6b0de2",
        "Yoshi no Tamago",
        "(Japan)",
        "DMG-YOJ",
        2607795318,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "86a7b2f79b03e7564223a8bddb476c6200607cf8",
        "Mario & Yoshi",
        "(Europe)",
        "DMG-YOX",
        1177016459,
        65536,
        "En",
        "Europe"
   , " " },
    {
        "36579c903eaa78e50af1b4dd8a3c46158afecfc7",
        "Pyramids of Ra",
        "(USA)",
        "DMG-YRE",
        2884573019,
        65536,
        "En",
        "USA"
   , " " },
    {
        "3af4e9bba7e3706b83fb5daccec251cb06244992",
        "Looney Tunes Series - Daffy Duck",
        "(Japan) (SGB)",
        "DMG-YSA",
        2039321737,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "a273474ccc65a026682f4dcacddf0a55417aafda",
        "Daffy Duck",
        "(USA, Europe) (SGB)",
        "DMG-YSE",
        3296096755,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "b9a13be8907f38657b3f09edecf73d4ad1cd827c",
        "Daffy Duck",
        "(Europe)",
        "DMG-YSX",
        333276285,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "f73434189311990716043c56891925a54db97432",
        "Donkey Kong Land",
        "(USA, Europe) (SGB)",
        "DMG-YTE",
        1239158071,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "40ed35e06f6a20d7defb2d81a0137f90b8372e79",
        "Super Donkey Kong GB",
        "(Japan) (SGB)",
        "DMG-YTJ",
        1767851520,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "3f1eaacaf0f699265fe4edcff2ee68a617fff3d0",
        "Double Yakuman",
        "(Japan)",
        "DMG-YVJ",
        623653557,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "f547d5e103883715e2f7c7743b6276538c26c2a1",
        "Kenyuu Densetsu Yaiba",
        "(Japan)",
        "DMG-YYJ",
        3424220814,
        262144,
        "Ja",
        "Japan"
   , " " },
    {
        "3683132e116d141fd52ff85127c2ac330862f6ba",
        "DuckTales 2",
        "(Japan)",
        "DMG-Z2J",
        728317150,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "c65ef5a502dd710cd5069e5a07ae1e1a76ada8aa",
        "Chikyuu Kaihou Gun ZAS",
        "(Japan) (En)",
        "DMG-ZAA",
        2080185599,
        131072,
        "En",
        "Japan"
   , " " },
    {
        "d69e41deaf31f25dc3434a7439911f841fc3155e",
        "Legend of Zelda, The - Link's Awakening",
        "(Canada) (Fr)",
        "DMG-ZCF",
        1141360473,
        524288,
        "Fr",
        "Canada"
   , " " },
    {
        "f2ac04727684110ebfc7de1a22edaf221537d69a",
        "Zerd no Densetsu",
        "(Japan)",
        "DMG-ZEJ",
        1227807542,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "831ea8ef3716864d95036ec47234f84af1a0c0ac",
        "Zool - Ninja of the 'Nth' Dimension",
        "(USA)",
        "DMG-ZGE",
        4015305838,
        131072,
        "En",
        "USA"
   , " " },
    {
        "a16187e168d658f7f09981130f6d0b34075a0e7c",
        "Zool - Ninja of the 'Nth' Dimension",
        "(Europe)",
        "DMG-ZGX",
        2920538663,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "171730bb043d901b95746b00dfacb9b2265d1da6",
        "Tarzan - Lord of the Jungle",
        "(USA, Europe)",
        "DMG-ZKE",
        3270840243,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "5c5b469cd1e76805e53e7fc3033660c7b118f928",
        "Legend of Zelda, The - Link's Awakening",
        "(Germany)",
        "DMG-ZLD",
        1980413159,
        524288,
        "De",
        "Germany"
   , " " },
    {
        "5d7c85dfeea6c7488655a96a0a2638da04536ce5",
        "Legend of Zelda, The - Link's Awakening",
        "(USA, Europe) (Rev 2)",
        "DMG-ZLE",
        886083195,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "913336b572723cd036c0fc136be6f34a7f0c885f",
        "Legend of Zelda, The - Link's Awakening",
        "(USA, Europe) (Rev 1)",
        "DMG-ZLE",
        2098949334,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "f7b089dec9666c267ecbd0445574a7fcf93df54d",
        "Legend of Zelda, The - Link's Awakening",
        "(USA, Europe)",
        "DMG-ZLE",
        2364701840,
        524288,
        "En",
        "USA, Europe"
   , " " },
    {
        "ea3430a92c09bb64bd9a5c9022a79afb139ad7f2",
        "Legend of Zelda, The - Link's Awakening",
        "(France)",
        "DMG-ZLF",
        3207246219,
        524288,
        "Fr",
        "France"
   , " " },
    {
        "6d4e0f8d393149582646ecad702d2146dadc6452",
        "Zelda no Densetsu - Yume o Miru Shima",
        "(Japan)",
        "DMG-ZLJ",
        967206990,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "dc7341c06d985e11be9c712e402ae58e14dfd16b",
        "Zelda no Densetsu - Yume o Miru Shima",
        "(Japan) (Rev 1)",
        "DMG-ZLJ",
        3928012842,
        524288,
        "Ja",
        "Japan"
   , " " },
    {
        "d47d5929ce13628ff797372f60a37cc5b718295b",
        "Taz-Mania",
        "(Europe)",
        "DMG-ZMX",
        140555879,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "3ef31fa1fcfd638efd39d0d2e07e114572603aea",
        "Zen - Intergalactic Ninja",
        "(USA)",
        "DMG-ZNE",
        2283341616,
        131072,
        "En",
        "USA"
   , " " },
    {
        "bfb85758fbcf9bdd0cb2e8473abdd50e7a929558",
        "Zen - Intergalactic Ninja",
        "(Europe)",
        "DMG-ZNX",
        1680637630,
        131072,
        "En",
        "Europe"
   , " " },
    {
        "c7843a94295c915119a71346a6be68709074aa4b",
        "Zoids Densetsu",
        "(Japan)",
        "DMG-ZOJ",
        3274557958,
        65536,
        "Ja",
        "Japan"
   , " " },
    {
        "a86da0d7d50af2c072a4630e1c593b01fbdf7aa2",
        "Pachi-Slot World Cup '94",
        "(Japan)",
        "DMG-ZPJ",
        2462651423,
        131072,
        "Ja",
        "Japan"
   , " " },
    {
        "da38c0a55686251c1494ba1efcc8c067c046da06",
        "InfoGenius Productivity Pak - Personal Organizer and Phone Book",
        "(USA)",
        "DMG-ZRE",
        3393415481,
        65536,
        "En",
        "USA"
   , " " },
    {
        "ccf7a9eee3e9b7ca37f7c47e6adbb6ccc4299886",
        "InfoGenius Systems - Personal Organizer with Phone Book",
        "(Europe)",
        "DMG-ZRX",
        3218494772,
        65536,
        "En",
        "Europe"
   , " " },
    {
        "8f992029e7c3ca43af8672e09a5ed74935e3b3c4",
        "Spider-Man 2",
        "(USA, Europe)",
        "DMG-ZSE",
        333259583,
        131072,
        "En",
        "USA, Europe"
   , " " },
    {
        "65727801e5e82298a8cdb19701845b8d75c37775",
        "Looney Tunes 2 - Tasmanian Devil in Island Chase",
        "(USA, Europe)",
        "DMG-ZTE",
        584088822,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "963c6cf25e7da975c80539dee3533d3ccf77245e",
        "Mighty Morphin Power Rangers",
        "(USA, Europe) (SGB)",
        "DMG-ZWE",
        3322741546,
        262144,
        "En",
        "USA, Europe"
   , " " },
    {
        "5d5b7ddcfbac99403924d50795a23b22cab0fcd2",
        "Shanghai",
        "(Japan) (Activision)",
        "DMG-ZZJ",
        1789297899,
        32768,
        "Ja",
        "Japan"
   , " " },
};


GameInfo search_gb_info_by_game_code(char *search_value) {
    
    int num_games = sizeof(games_info) / sizeof(GameInfo);
    for (int i = 0; i < num_games; i++) {
        int result1 = strcmp(games_info[i].key, search_value);
        if (result1 == 0) {
            return games_info[i];  // Return the index of the matching game
        }
    }
    GameInfo null_info = {
        "NULL",
        "NULL",
        "NULL",
        "NULL",
        0,
        0,
        "NULL",
        "NULL",
        "NULL"
    };
    return null_info;  // Return -1 if no match is found
}