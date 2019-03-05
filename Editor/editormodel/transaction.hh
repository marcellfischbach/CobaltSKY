

#pragma once

#include <editormodel/editormodelexport.hh>
#include <functional>
#include <list>
#include <exception>

namespace cs::editor::model
{

  class EDITOR_MODEL_API IllegalTransactionStateException : public std::exception
  {
  public:
    IllegalTransactionStateException(const std::string &message);
    virtual ~IllegalTransactionStateException();

    const std::string &GetMessage() const;

  private:
    std::string m_message;
  };




  class EDITOR_MODEL_API Transaction
  {
    friend class Model;
  public:
    enum State
    {
      eS_Undefined,
      eS_Open,
      eS_Committed,
      eS_Rolledback,
      eS_Invalid

    };
  public:
    ~Transaction();

    void OnBegin(std::function<void()> func);
    void OnCommit(std::function<void()> func);
    void OnRollback(std::function<void()> func);

    void Begin();
    void Commit();
    void Rollback();

    bool IsValid() const;


    unsigned long GetTxID() const;


  private:
    Transaction();
    Transaction(State state);

    std::list<std::function<void()>> m_commits;
    std::list<std::function<void()>> m_rollbacks;

    State m_state;

    unsigned long m_txID;
  };


}
