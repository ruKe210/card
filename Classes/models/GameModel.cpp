#include "GameModel.h"
#include "cocos2d.h"

using namespace cocos2d;

GameModel::GameModel()
{
    _currentTrayCard = nullptr;

}

GameModel::~GameModel()
{
    // 所有卡牌由 _cardIdMap 统一管理，逐一释放
    for (auto &p : _cardIdMap) {
        delete p.second;
    }
    _cardIdMap.clear();
    _playfieldCards.clear();
    _stackCards.clear();
    _currentTrayCard = nullptr;
}

void GameModel::addPlayfieldCard(CardModel* card)
{
    if (!card) return;
    _playfieldCards.push_back(card);
    _cardIdMap[card->getCardId()] = card;
}

void GameModel::addStackCard(CardModel* card)
{
    if (!card) return;
    _stackCards.push_back(card);
    _cardIdMap[card->getCardId()] = card;
    _stackTopCard = _stackCards.size() - 2;
}

const std::vector<CardModel*>& GameModel::getPlayfieldCards() const
{
    return _playfieldCards;
}

const std::vector<CardModel*>& GameModel::getStackCards() const
{
    return _stackCards;
}

CardModel* GameModel::getCurrentTrayCard() const
{
    return _currentTrayCard;
}

void GameModel::setCurrentTrayCard(CardModel* card)
{
    _currentTrayCard = card;
    if (card) {
        _cardIdMap[card->getCardId()] = card;
    }
}

CardModel* GameModel::drawStackCard()
{
    if (_stackTopCard < 0)
        return nullptr;
    CardModel* card = _stackCards.at(_stackTopCard--);
    return card;
}
CardModel* GameModel::peekStackCard()
{
    if (_stackTopCard < 0)
        return nullptr;
    CardModel* card = _stackCards.at(_stackTopCard);
    return card;

}

void GameModel::pushStackCard()
{
    _stackTopCard++;
    return;
}

CardModel* GameModel::findCardById(int cardId) const
{
    auto it = _cardIdMap.find(cardId);
    if (it == _cardIdMap.end()) return nullptr;
    return it->second;
}

void  GameModel::fromJson(const std::string& jsonPath)
{
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(jsonPath);
    std::string content = FileUtils::getInstance()->getStringFromFile(fullPath);
    if (content.empty()) 
        return ;

    rapidjson::Document doc;
    doc.Parse(content.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        return;
    }

    int id = 0;

    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray()) {
        const rapidjson::Value& playfieldArr = doc["Playfield"];
        for (rapidjson::SizeType i = 0; i < playfieldArr.Size(); ++i) {
            const rapidjson::Value& cardObj = playfieldArr[i];
            
            CardFaceType cardFace = (CardFaceType)cardObj["CardFace"].GetInt();
            CardSuitType cardSuit = (CardSuitType)cardObj["CardSuit"].GetInt();
            Vec2 cardPosition(cardObj["Position"]["x"].GetDouble(), cardObj["Position"]["y"].GetDouble());
            CardModel* card = new CardModel(id++, cardFace, cardSuit, cardPosition);
   

            addPlayfieldCard(card);
            
          
        }
    }
    if (doc.HasMember("Stack") && doc["Stack"].IsArray())
    {
        const rapidjson::Value& playfieldArr = doc["Stack"];
        for (rapidjson::SizeType i = 0; i < playfieldArr.Size(); ++i) {
            const rapidjson::Value& cardObj = playfieldArr[i];

            CardFaceType cardFace = (CardFaceType)cardObj["CardFace"].GetInt();
            CardSuitType cardSuit = (CardSuitType)cardObj["CardSuit"].GetInt();
            Vec2 cardPosition(cardObj["Position"]["x"].GetDouble(), cardObj["Position"]["y"].GetDouble());
            CardModel* card = new CardModel(id++, cardFace, cardSuit, cardPosition);

            addStackCard(card);


        }
    }

    return;
}
