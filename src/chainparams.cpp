// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "assert.h"

#include "chainparams.h"
#include "main.h"
#include "init.h"
#include "net.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

int64_t CChainParams::GetProofOfWorkReward(int nHeight, int64_t nFees) const
{
    // miner's coin base reward
     int64_t nSubsidy = (NetworkID() == CChainParams::TESTNET ? 0 : 0 ) * COIN;

    if (nHeight >= 1 && nHeight <= 100)
	{
        nSubsidy =  60000000 * COIN;
	}
	else
    {		
        nSubsidy =  0 * COIN;		
	}	
    if (fDebug && GetBoolArg("-printcreation"))
        LogPrintf("GetProofOfWorkReward() : create=%s nSubsidy=%d\n", FormatMoney(nSubsidy).c_str(), nSubsidy);

    return nSubsidy + nFees;
};


int64_t CChainParams::GetProofOfStakeReward(const CBlockIndex* pindexPrev, int64_t nCoinAge, int64_t nFees) const
{
    // miner's coin stake reward based on coin age spent (coin-days)
    int64_t nSubsidy;
    nSubsidy = (pindexPrev->nMoneySupply / COIN) * COIN_YEAR_REWARD() / (365 * 24 * (60 * 60 / 64));

    if (fDebug && GetBoolArg("-printcreation"))
        LogPrintf("GetProofOfStakeReward(): create=%s nCoinAge=%d Percentage=%d\n", FormatMoney(nSubsidy).c_str(), nCoinAge, COIN_YEAR_REWARD());

    return nSubsidy + nFees;
}

int64_t CChainParams::COIN_YEAR_REWARD() const
{
	int64_t Balance = pwalletMain->GetBalance();
    if (Balance <= (1000 * COIN)) {
		return 2.18 * CENT;
	} else if (Balance <= (5000 * COIN)) {
		return 2.18 * CENT;
	} else if (Balance <= (50000 * COIN)) {
		return 2.18 * CENT;
	} else {
		return 2.18 * CENT;
	}
}

//
// Main network
//

// Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

// Convert the pnSeeds array into usable address objects.
static void convertSeeds(std::vector<CAddress> &vSeedsOut, const unsigned int *data, unsigned int count, int port)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int k = 0; k < count; ++k)
    {
        struct in_addr ip;
        unsigned int i = data[k], t;

        // -- convert to big endian
        t =   (i & 0x000000ff) << 24u
            | (i & 0x0000ff00) << 8u
            | (i & 0x00ff0000) >> 8u
            | (i & 0xff000000) >> 24u;

        memcpy(&ip, &t, sizeof(ip));

        CAddress addr(CService(ip, port));
        addr.nTime = GetTime()-GetRand(nOneWeek)-nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

class CBaseChainParams : public CChainParams {
public:
    CBaseChainParams() {
        const char* pszTimestamp = "ChipCoin";
        CTransaction txNew;
        txNew.nTime = GENESIS_BLOCK_TIME;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 0 << CBigNum(42) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].SetEmpty();
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = GENESIS_BLOCK_TIME;

        vSeeds.push_back(CDNSSeedData("seed1.chipcoin.co", "seed1.chipcoin.co"));
        vSeeds.push_back(CDNSSeedData("seed2.chipcoin.co", "seed2.chipcoin.co"));
        vSeeds.push_back(CDNSSeedData("seed3.chipcoin.co", "seed3.chipcoin.co"));
        vSeeds.push_back(CDNSSeedData("seed4.chipcoin.co", "seed4.chipcoin.co"));
    }
    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual const std::vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    std::vector<CAddress> vFixedSeeds;
};

class CMainParams : public CBaseChainParams {
public:
    CMainParams() {
        strNetworkID = "main";

        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0x61;
        pchMessageStart[1] = 0x62;
        pchMessageStart[2] = 0x73;
        pchMessageStart[3] = 0x84;

        vAlertPubKey = ParseHex("0417d2930330031da4a92bd846c802a678afd654ba3924b4fdcee93279f8e5182fd67c7c00a5590aa9e268d59be8aa3930b5d191685764dab8c0c6fe7b6c8f89da");

        nDefaultPort = 15156;
        nRPCPort = 15155;
        nBIP44ID = 0x80000025;

        nLastPOWBlock = 5000;
        nFirstPosv2Block = 120;
        nFirstPosv3Block = 240;

        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 20); // "standard" scrypt target limit for proof of work, results with 0,000244140625 proof-of-work difficulty
        bnProofOfStakeLimit = CBigNum(~uint256(0) >> 20);
        bnProofOfStakeLimitV2 = CBigNum(~uint256(0) >> 48);

        genesis.nBits    = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce   = 6107494;
		
/*		if (true && genesis.GetHash() != hashGenesisBlock)
                       {
                           printf("Searching for genesis block...\n");
                           uint256 hashTarget = CBigNum().SetCompact(genesis.nBits).getuint256();
                           uint256 thash;
                           while (true)
                           {
                               thash = genesis.GetHash();
                               if (thash <= hashTarget)
                                 break;
                               if ((genesis.nNonce & 0xFFF) == 0)
                               {
                                   printf("nonce %08X: hash = %s (target = %s)\n", genesis.nNonce, thash.ToString().c_str(), hashTarget.ToString().c_str());
                               }
                               ++genesis.nNonce;
                               if (genesis.nNonce == 0)
                               {
                                   printf("NONCE WRAPPED, incrementing time\n");
                                   ++genesis.nTime;
                               }
                           }
                           printf("genesis.nTime = %u \n", genesis.nTime);
                           printf("genesis.nNonce = %u \n", genesis.nNonce);
                           printf("genesis.nVersion = %u \n", genesis.nVersion);
                           printf("genesis.GetHash = %s\n", genesis.GetHash().ToString().c_str()); //first this, then comment this line out and uncomment the one under.
                           printf("genesis.hashMerkleRoot = %s \n", genesis.hashMerkleRoot.ToString().c_str()); //improvised. worked for me, to find merkle root/
                       }
*/		
        hashGenesisBlock = genesis.GetHash();

        assert(hashGenesisBlock == uint256("0x000005264469eb66d36845272ca444cfe0693ca5218724cddfdffe048ca0d576"));
        assert(genesis.hashMerkleRoot == uint256("0x941e8d021280d466d6fe8d3dd43d54acc763d6e7e99638246d0176790d0e21eb"));

        base58Prefixes[PUBKEY_ADDRESS]      = list_of(28).convert_to_container<std::vector<unsigned char> >(); // 8
        base58Prefixes[SCRIPT_ADDRESS]      = list_of(87).convert_to_container<std::vector<unsigned char> >(); // c
        base58Prefixes[SECRET_KEY]          = list_of(115).convert_to_container<std::vector<unsigned char> >(); // 128+ 16
        base58Prefixes[STEALTH_ADDRESS]     = list_of(63).convert_to_container<std::vector<unsigned char> >();  // S for Stealth addresses
        base58Prefixes[EXT_PUBLIC_KEY]      = list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY]      = list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_KEY_HASH]        = list_of(118).convert_to_container<std::vector<unsigned char> >();         // p
        base58Prefixes[EXT_ACC_HASH]        = list_of(120).convert_to_container<std::vector<unsigned char> >();          // q
        base58Prefixes[EXT_PUBLIC_KEY_BTC]  = list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >(); // xprv
        base58Prefixes[EXT_SECRET_KEY_BTC]  = list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >(); // xpub

        //convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));
        convertSeeds(vFixedSeeds, pnSeed, ARRAYLEN(pnSeed), nDefaultPort);
    }

    virtual Network NetworkID() const { return CChainParams::MAIN; }
};
static CMainParams mainParams;

//
// Testnet
//

class CTestNetParams : public CBaseChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        strDataDir = "testnet";

        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0x8c;
        pchMessageStart[1] = 0x96;
        pchMessageStart[2] = 0xa0;
        pchMessageStart[3] = 0xaa;

        vAlertPubKey = ParseHex("0411da4157a771603c0c0e06d178b369f3710d970a38fb5f9f6f316774cd142a28bded559276508036bfe842098a2dd3cafe7b1fb3322f09c198f8c0854dcf9920");

        nDefaultPort = 15840;
        nRPCPort = 15211;
        nBIP44ID = 0x80000001;

		    nLastPOWBlock = 86400;
        nFirstPosv2Block = 120;
        nFirstPosv3Block = 240;

        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 16);
        bnProofOfStakeLimit = CBigNum(~uint256(0) >> 20);
        bnProofOfStakeLimitV2 = CBigNum(~uint256(0) >> 16);

        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 88324;
/*		
	if (true && genesis.GetHash() != hashGenesisBlock)
                       {
                           printf("Searching for genesis block...\n");
                           uint256 hashTarget = CBigNum().SetCompact(genesis.nBits).getuint256();
                           uint256 thash;
                           while (true)
                           {
                               thash = genesis.GetHash();
                               if (thash <= hashTarget)
                                 break;
                               if ((genesis.nNonce & 0xFFF) == 0)
                               {
                                   printf("nonce %08X: hash = %s (target = %s)\n", genesis.nNonce, thash.ToString().c_str(), hashTarget.ToString().c_str());
                               }
                               ++genesis.nNonce;
                               if (genesis.nNonce == 0)
                               {
                                   printf("NONCE WRAPPED, incrementing time\n");
                                   ++genesis.nTime;
                               }
                           }
                           printf("genesis.nTime = %u \n", genesis.nTime);
                           printf("genesis.nNonce = %u \n", genesis.nNonce);
                           printf("genesis.nVersion = %u \n", genesis.nVersion);
                           printf("genesis.GetHash = %s\n", genesis.GetHash().ToString().c_str()); //first this, then comment this line out and uncomment the one under.
                           printf("genesis.hashMerkleRoot = %s \n", genesis.hashMerkleRoot.ToString().c_str()); //improvised. worked for me, to find merkle root/
                       }
*/		
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x0000804e38c7f7205fb080a5568a7bed73f84f8f170ed74a95b763939d126a14"));

        base58Prefixes[PUBKEY_ADDRESS]      = list_of(66).convert_to_container<std::vector<unsigned char> >(); // T
        base58Prefixes[SCRIPT_ADDRESS]      = list_of(76).convert_to_container<std::vector<unsigned char> >(); //X
        base58Prefixes[SECRET_KEY]          = list_of(194).convert_to_container<std::vector<unsigned char> >(); // 66 + 128
        base58Prefixes[STEALTH_ADDRESS]     = list_of(40).convert_to_container<std::vector<unsigned char> >(); // s for testnet stealth addresses
        base58Prefixes[EXT_PUBLIC_KEY]      = list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY]      = list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_KEY_HASH]        = list_of(56).convert_to_container<std::vector<unsigned char> >();          // P
        base58Prefixes[EXT_ACC_HASH]        = list_of(58).convert_to_container<std::vector<unsigned char> >();          // Q
        base58Prefixes[EXT_PUBLIC_KEY_BTC]  = list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >(); // tprv
        base58Prefixes[EXT_SECRET_KEY_BTC]  = list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >(); // tpub

        //convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));
        convertSeeds(vFixedSeeds, pnTestnetSeed, ARRAYLEN(pnTestnetSeed), nDefaultPort);
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


//
// Regression test
//
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        strDataDir = "regtest";

        nFirstPosv2Block = -1;
        nFirstPosv3Block = -1;

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 1);
        genesis.nTime = 1517035308;
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 3;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 15841;
 /*        if (true)
                         {
                             printf("Searching for genesis block...\n");
                             uint256 hashTarget = CBigNum().SetCompact(genesis.nBits).getuint256();
                             uint256 thash;
                             while (true)
                             {
                                 thash = genesis.GetHash();
                                 if (thash <= hashTarget)
                                     break;
                                 if ((genesis.nNonce & 0xFFF) == 0)
                                 {
                                     printf("nonce %08X: hash = %s (target = %s)\n", genesis.nNonce, thash.ToString().c_str(), hashTarget.ToString().c_str());
                                 }
                                 ++genesis.nNonce;
                                 if (genesis.nNonce == 0)
                                 {
                                     printf("NONCE WRAPPED, incrementing time\n");
                                     ++genesis.nTime;
                                 }
                             }
                             printf("genesis.nTime = %u \n", genesis.nTime);
                             printf("genesis.nNonce = %u \n", genesis.nNonce);
                             printf("genesis.nVersion = %u \n", genesis.nVersion);
                             printf("genesis.hashMerkleRoot = %s \n", genesis.hashMerkleRoot.ToString().c_str()); //idk
                             printf("genesis.GetHash = %s\n", genesis.GetHash().ToString().c_str());
                         }
*/
						 assert(hashGenesisBlock == uint256("0x3ae5b0bedb00f84ad11ef18a12944f16b27c9d75f4ba208b0629c869b46ffc59"));

        vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.
    }

    virtual bool RequireRPCPassword() const { return false; }
    virtual Network NetworkID() const { return CChainParams::REGTEST; }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

const CChainParams &TestNetParams() {
    return testNetParams;
}

const CChainParams &MainNetParams() {
    return mainParams;
}

void SelectParams(CChainParams::Network network)
{
    switch (network)
    {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        case CChainParams::REGTEST:
            pCurrentParams = &regTestParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    };
};

bool SelectParamsFromCommandLine()
{
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet && fRegTest)
    {
        return false;
    };

    if (fRegTest)
    {
        SelectParams(CChainParams::REGTEST);
    } else
    if (fTestNet)
    {
        SelectParams(CChainParams::TESTNET);
    } else
    {
        SelectParams(CChainParams::MAIN);
    };

    return true;
}
