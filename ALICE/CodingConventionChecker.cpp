// ALICE O2 coding conventions
// Shyam Kumar; shyam.kumar@cern.ch
#include "CodingConventionChecker.h"

#include <cctype>
// isupper

#include <clang/AST/ASTContext.h>
using clang::ASTContext;

#include <clang/AST/Comment.h>
using clang::comments::FullComment;
using clang::comments::BlockContentComment;
using clang::comments::VerbatimLineComment;
using clang::comments::ParagraphComment;
using clang::comments::TextComment;

#include <clang/AST/Decl.h>
using clang::VarDecl;

#include <clang/AST/DeclBase.h>
typedef clang::Decl::attr_iterator attr_iterator;

#include <clang/StaticAnalyzer/Core/BugReporter/BugReporter.h>
using clang::ento::BugReporter;
#ifdef USE_BUGTYPE
using clang::ento::BugReport;
#endif // USE_BUGTYPE

#ifdef USE_BUGTYPE
#include <clang/StaticAnalyzer/Core/BugReporter/BugType.h>
using clang::ento::BugType;
#endif // USE_BUGTYPE

#include <clang/StaticAnalyzer/Core/BugReporter/PathDiagnostic.h>
using clang::ento::PathDiagnosticLocation;

#include <clang/StaticAnalyzer/Core/Checker.h>
using clang::ento::Checker;
using clang::ento::check::ASTDecl;

#include <clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h>
using clang::ento::AnalysisManager;

#include <llvm/ADT/StringRef.h>
using llvm::StringRef;

#include <llvm/ADT/ArrayRef.h>
using llvm::ArrayRef;

#include <llvm/Support/Casting.h>
using llvm::dyn_cast;

namespace sas
{
namespace ALICE{
   const char* const CodingConventionChecker::checkerName = "sas.ALICE.Code";
    //----------------------Variable Name Checker, Underscore, Camelcase, Lowercase, Meaningful name----------------------------------
   void CodingConventionChecker::checkASTDecl(const VarDecl* D, AnalysisManager& Mgr, BugReporter& BR) const
   {
      //------Name of the declared variable:----------
      const StringRef Name = D->getName();
      const char* const NameChar = Name.data();
      // First letter of the name:
      const char FirstLetter = NameChar[0];
      const int size= Name.size();  
      std::string fullname = D->getNameAsString();
      std::string::size_type m;
      std::string const str = fullname; 
      m = str.find("_");
     // Don't emit report if FirstLetter is uppercase:
      if (size==1)
      Report(D, "[ALICE O2 Coding Conventions: Variable] Variable name should be meaningful and should have atleast two alphabet", BR);
      if (isupper(FirstLetter)) 
      Report(D, "[ALICE O2 Coding Conventions: Variable] Variable name should begin with a Lower case letter", BR);
      else 
        for (int i=0; i<size; i++)
      {
       bool flag1=false;
       bool flag2=false;
       const char Letter=NameChar[i];
       if (islower(Letter)) 
       flag1=true;
       else
       flag2=true;
       if(flag1&&flag2) return;
       Report(D, "[ALICE O2 Coding Conventions: Variable] variable name should follow the camel case convention", BR);
       }
      if (!isalnum(FirstLetter)) return; 
      Report(D, "[ALICE O2 Coding Conventions: Variable] Variable name doesn't begin with an Alphanumeric letter", BR);
      if (m == std::string::npos)  return;
      Report(D, "[ALICE O2 Coding Conventions: Variable] Variable name does not contain underscore", BR);
      D=NULL;
      delete D; 
   }
    
   
  //----------------------Function Declaration Checker----------------------------------
    void CodingConventionChecker::checkASTDecl(const clang::FunctionDecl* D, clang::ento::AnalysisManager &Mgr, clang::ento::BugReporter &BR) const
        { 
         //  std::cout<<"============================================="<<std::endl;
            std::string funcname = D->getNameAsString();
            std::string::size_type m;
            std::string const str = funcname; 
            m = str.find("_"); 	
            const int size= funcname.size();
            const bool pure=D->isPure();
            const bool global=D->isGlobal();
            const bool noreturn=D->isNoReturn();
            if (!pure)
            Report(D, "[ALICE O2 Coding Conventions: Function]  function defined is not virtual", BR);
            if (!noreturn)
            {
             Report(D, "[ALICE O2 Coding Conventions: Function] return type function", BR);
            }
            if (global){
           //  std::cout<<" The name of function:"<<gName<<std::endl;
             Report(D, "[ALICE O2 Coding Conventions: Function] function is global", BR);
            }
            if (funcname.size()==1) 
            Report(D, "[ALICE O2 Coding Conventions: Function] function name  must be meaningful", BR);
            std::string s(funcname);
            std::string t("get");
            std::string u("set");
            if (s.compare(0, t.length(), t) != 0) 
            Report(D, "[ALICE O2 Coding Conventions: Function] The usage of get function detected.", BR);
            if (s.compare(0, u.length(), u) != 0) 
            Report(D, "[ALICE O2 Coding Conventions: Function] The usage of set function detected.", BR);
            if (m != std::string::npos) 
            Report(D, "[ALICE O2 Coding Conventions: Function] function name does not contain underscore", BR);
            if (funcname == "main") return;
            auto firstChar  = funcname[0];
            if (firstChar!='~'&& isupper(firstChar)) 
            Report(D, "[ALICE O2 Coding Conventions: Function] function name should begin with a Lower case letter", BR);
            else 
            for (int i=0; i<size; i++) {
            bool flag1=false;
            bool flag2=false;
            const char Letter=funcname[i];
            if (islower(Letter)) 
            flag1=true;
            else
            flag2=true;
            if(flag1&&flag2) return;
            Report(D, "[ALICE O2 Coding Conventions: Function] function name should follow the camel case convention", BR);
            }
           
            D=NULL;
            delete D; 
         }
//-----------------------Default Constructor Checker---------------------------------------
     void CodingConventionChecker::checkASTDecl(const clang::CXXConstructorDecl* D, clang::ento::AnalysisManager& Mgr, clang::ento::BugReporter& BR) const
         {
            std::string constructname = D->getNameAsString();
            const bool defconst=D->isDefaultConstructor();
            const bool copyconst=D->isCopyConstructor();
            if (defconst) 
            Report(D, "[ALICE O2 Coding Conventions: Constructor] Default constructor of class detected.", BR);
            if (copyconst) {
            Report(D, "[ALICE O2 Coding Conventions: Constructor] Copy constructor of class detected.", BR);
            }
            D=NULL;
            delete D;
         }
//-----------------------Destructor Checker---------------------------------------
     void CodingConventionChecker::checkASTDecl(const clang::CXXDestructorDecl* D, clang::ento::AnalysisManager& Mgr, clang::ento::BugReporter& BR) const
         {
            std::string destructname = D->getNameAsString();
            auto firstChar=destructname[0];
            if (firstChar=='~') 
            Report(D, "[ALICE O2 Coding Conventions: Destructor] Destructor of class detected.", BR);
       
            D=NULL;
            delete D;
         }
   //------------------------Enum Declataion Checker----------------------------------- 
     void CodingConventionChecker::checkASTDecl(const clang::EnumDecl* D, clang::ento::AnalysisManager& Mgr, clang::ento::BugReporter& BR) const
         {
           // auto declName=D->getName();
            const StringRef declName=D->getName(); 
            if (declName.size()<2 || declName.startswith("e")) 
            Report(D, "[ALICE O2 Coding Conventions: Enum] The first letter of Enumerations names must be \"e\".", BR);
            D=NULL;
            delete D;
         }
 
   //-----------------------Field Declaration Checker---------------------------------------
     void CodingConventionChecker::checkASTDecl(const clang::FieldDecl* D, clang::ento::AnalysisManager& Mgr, clang::ento::BugReporter& BR) const
         {
            std::string fieldname = D->getName();
           // std::cout<<" The name of function:"<<fieldname<<std::endl;
            if (fieldname.size()<=2) return;
            auto firstChar = fieldname[0];
            auto secondChar = fieldname[1];
            if ( firstChar == 'f' && std::isupper(secondChar) ) return;
            Report(D, "[ALICE O2 Coding Conventions: Field] The first letter of the variable must be f and the second letter a capital letter.", BR);
            D=NULL;
            delete D;
         } 
   //-----------------------Name Space Checker---------------------------------------
     void CodingConventionChecker::checkASTDecl(const clang::UsingDirectiveDecl *D, clang::ento::AnalysisManager &Mgr, clang::ento::BugReporter &BR) const
         {
        
            if (isDeclOK(D, BR)) return;
            Report(D, "[ALICE O2 Coding Conventions: Directive] Please don't use 'using namespace' in a header and source file", BR);
            D=NULL;
            delete D;
         }

   // using std::*
         void CodingConventionChecker::checkASTDecl(const clang::UsingDecl *D, clang::ento::AnalysisManager &Mgr, clang::ento::BugReporter &BR) const
         {
            if (!D) 
           // std::cout<<"The name of String:"<<std::endl;
            return;
           if (isDeclOK(D, BR)) return;
            std::string NS = D->getQualifier()->getAsNamespace()->getNameAsString();
            std::cout<<"The name of String:"<<NS<<std::endl;
            if (strcmp(NS.c_str(), "std") != 0) return; // Different from "std"
            Report(D, "[ALICE O2 Coding Conventions:  Declaration] Detected usage of 'using std::' in a header.", BR);
            D=NULL;
            delete D;
         }

      // Checking the file name of the header and source
         bool CodingConventionChecker::isDeclOK(const clang::NamedDecl *D, clang::ento::BugReporter &BR) const
         {  
            if (!D) return true;                                                                      // Invalid declaration
            if (D->getDeclContext()->getParent() != 0) return true;                                   // Not on top level in AST
            std::string sfile = BR.getSourceManager().getPresumedLoc(D->getLocation()).getFilename(); // Get filename
            std::string::size_type m;
            std::string::size_type n;
            std::string::size_type p;
            std::string::size_type r;
            std::string const str = sfile; 
            m = str.find(".cpp");
            n = str.find(".h");
            p = str.find(".H");
            r = str.find (".cxx");
            if (m ||n||p||r)
            return false;
            return true;
         }
        void CodingConventionChecker::checkPreStmt(const clang::CallExpr *E,
                                                clang::ento::CheckerContext &C) const
         {
            for(clang::Stmt::const_child_iterator I = E->child_begin(), F = E->child_end();
                I != F;
                ++I)
            {
               const clang::Expr *child = llvm::dyn_cast<clang::Expr>(*I);
               if(!child) continue;
               if(llvm::isa<clang::DeclRefExpr>(child->IgnoreImpCasts())) {
                  const clang::NamedDecl *ND =
                     llvm::cast<clang::DeclRefExpr>(child->IgnoreImpCasts())->getFoundDecl();
                  std::string identifierName = ND->getNameAsString();
                  if(identifierName == "cerr" ||
                     identifierName == "cout" ||
                     identifierName == "printf")
                  {
                     const char* msg = "[ALICE O2 Coding Conventions: Declaration ] The usage of printouts may be considered an issue.";
                     Report(E, msg, C);
                  }
               }

            }
         }
//------------------------------New Checker-----------------------------------------------------
} 
} 
