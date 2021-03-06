

/*
 * TODO:
 *
 * Functions and procedure return type distinction
 * DONE - Procedure and function calls with no args
 * DONE?- Constants
 * DONE - While Loop NOPE, look at output from factorization.p
 * Arrays and actually specifing types
 * DONE - To and Downto
 * DONE - readln
 * DONE - break and exit
 * DONE mod div and other operators and or
 *
 * Samples problems
 *  output from factorization.p. looks like problem with while loops
 *  DONE gcd and or wont work
 *  dec() function in some places 
 *  inc()
 */
#include "ast.h"

#include "llvm/ADT/APSInt.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/Value.h"

using namespace llvm;

static llvm::LLVMContext theContext;
static llvm::IRBuilder<> builder(theContext);
static std::unique_ptr<llvm::Module> theModule;
static std::map<std::string, AllocaInst*> namedValues;
static std::vector<std::string> globalConstants;

static AllocaInst *CreateEntryBlockAlloca(Function *theFunction,
        const std::string &VarName){
    IRBuilder<> TmpB(&theFunction->getEntryBlock(),
            theFunction->getEntryBlock().begin());
    return TmpB.CreateAlloca(Type::getInt64Ty(theContext), 0, VarName.c_str());
}

//This is a really shitty way to do this.
//But I think I'd have to rework a lot of my codegen otherwise
//And I don't have time to do that
static BasicBlock* lastFunctionReturnBlock;
static bool hasBrokeFromFunctionInBlock;
static BasicBlock* lastLoopEndBlock;
static bool hasBrokeFromLoopInBlock;

Value* LogErrorV(const char *str){
    fprintf(stderr, "Error: %s\n", str);
    return nullptr;
}


Value* MainBlockAST::codegen(){
    //Remember I want to call the DoAllocations on the declarations not code gen. will need to cast
    std::vector<AllocaInst *> OldBindings;
    for(int i = 0; i<declarations.size(); i++){
        auto decl = dynamic_cast<DeclarationAST*>(declarations[i].get());
        if(!decl){
            printf("DeclarationAST cast failed");
            return nullptr;
        }
        if(PrototypeAST *proto = dynamic_cast<PrototypeAST*>(decl)){
            proto->codegen();
        }
        else{
            auto oldBind = decl->DoAllocations();
            OldBindings.insert(OldBindings.end(), oldBind.begin(), oldBind.end());
        }
    }
   
    Value* BodyVal = statementSequence->codegen();
    if(!BodyVal)
        return nullptr;

    for(int i = 0; i<OldBindings.size();i++){
        //This won't work cuz it allows for vars to be accessed from an already deleted scope
        if(OldBindings[i])
            namedValues[OldBindings[i]->getName()] = OldBindings[i];
    }

    return BodyVal;
}

Value* ProgramAST::codegen(){
    theModule = llvm::make_unique<Module>(programName, theContext);

    FunctionType *FT = FunctionType::get(Type::getVoidTy(theContext), false);
    Function *F = Function::Create(FT, Function::ExternalLinkage, "main", theModule.get());
    BasicBlock *BB = BasicBlock::Create(theContext, "entry", F);
    builder.SetInsertPoint(BB);

    for(int i = 0; i<declarations.size(); i++){
        auto decl = dynamic_cast<DeclarationAST*>(declarations[i].get());
        if(!decl){
            printf("DeclarationAST cast failed");
            return nullptr;
        }
        if(PrototypeAST *proto = dynamic_cast<PrototypeAST*>(decl)){
            proto->codegen();
        }
        else{
            auto oldBind = decl->DoAllocations();
        }
        builder.SetInsertPoint(BB);
    }
    Value* BodyVal = statementSequence->codegen();
    if(!BodyVal)
        return nullptr;

    builder.CreateRetVoid();
    verifyFunction(*F);

    theModule->print(errs(), nullptr);
    llvmModule = std::move(theModule);
    return BodyVal;
}

Value* StatementSequenceAST::codegen(){
    for(int i = 0; i<statements.size(); i++){
        if(!hasBrokeFromLoopInBlock && !hasBrokeFromFunctionInBlock){
            statements[i]->codegen();
        }
    }
    return Constant::getNullValue(Type::getInt64Ty(theContext));
}

Value* NumberAST::codegen(){
    return ConstantInt::get(theContext, APInt(64, value));
}

Value* VariableIdentifierAST::codegen(){
    Value* v = namedValues[name];
    if(!v){
        printf("Unknown variable name %s\n", name.c_str());
        return nullptr;
    }

    return builder.CreateLoad(v, name.c_str());
}

Value* UnaryOpAST::codegen(){
    return expression->codegen();
}

// So I think what I should do instead is create a variable with the same name as the function
// So assignment still works, and everything. then when returning, we just return what is currently
// assigned to that variable and remove the variable from the named values list.

Value* BinaryOpAST::codegen(){
    Value* L = LHS->codegen();
    Value* R = RHS->codegen();
    if(!L || !R)
        return nullptr;

    switch(op){
        case PLUS: return builder.CreateAdd(L, R, "addtmp");
        case MINUS: return builder.CreateSub(L, R, "subtmp");
        case TIMES: return builder.CreateMul(L, R, "multmp");
        case DIV: return builder.CreateUDiv(L, R, "divtmp");
        case AND: return builder.CreateAnd(L, R, "andtmp");
        case OR: return builder.CreateOr(L, R, "ortmp");                   
        case MOD: return builder.CreateURem(L, R, "modtmp");
        case ASSIGN:
            {
                printf("ASSIGNMENT\n");
                VariableIdentifierAST *LHSE = dynamic_cast<VariableIdentifierAST*>(LHS.get());
                if(!LHSE)
                    return LogErrorV("left hand side of assignment must be a varaible");
                if(std::find(globalConstants.begin(), globalConstants.end(), LHSE->GetName()) !=globalConstants.end())
                    return LogErrorV("Cannot assign to a constant!");
                
                Value *Variable = namedValues[LHSE->GetName()];
                if(!Variable){
                    printf("Unknown variable name %s\n", LHSE->GetName().c_str()); 
                    return nullptr;
                }

                builder.CreateStore(R, Variable);
                return R;
            }
        default:{printf("Invalid operator %s\n", lexicalTokenNames[op]); return nullptr; } 
    }
}

Value* ComparisonOpAST::codegen(){
    Value* L = LHS->codegen();
    Value* R = RHS->codegen();
    if(!L || !R)
        return nullptr;

    switch(op){    
            //L = Builder.CreateFCmpULT(L, R, "cmptmp");
            //return Builder.CreateUIToFP(L, Type::getDoubleTy(TheContext), "booltmp");
        case LESSTHAN: return builder.CreateICmpULT(L, R, "cmptmp");
        case LESSTHANEQ: return builder.CreateICmpULE(L, R, "cmptmp");
        case GREATERTHAN: return builder.CreateICmpUGT(L, R, "cmptmp");
        case GREATERTHANEQ: return builder.CreateICmpUGE(L, R, "cmptmp");
        case NOTEQUAL: return builder.CreateICmpNE(L, R, "cmptmp");
        case EQUAL: return builder.CreateICmpEQ(L, R, "cmptmp");
        default:{printf("Invalid comparison operator %s\n", lexicalTokenNames[op]); return nullptr; } 
    }
}

Value* ExitBreakStatementAST::codegen(){
    if(exitOrBreak == KW_EXIT){
        builder.CreateBr(lastFunctionReturnBlock);
        hasBrokeFromFunctionInBlock = true;
        return Constant::getNullValue(Type::getInt64Ty(theContext));
    }
    else{
        builder.CreateBr(lastLoopEndBlock);
        hasBrokeFromLoopInBlock = true;
        return Constant::getNullValue(Type::getInt64Ty(theContext));
    }
}

std::vector<AllocaInst*> VariableDeclarationsOfTypeAST::DoAllocations(){
    std::vector<AllocaInst *> OldBindings;

    Function *TheFunction = builder.GetInsertBlock()->getParent();

    for(int i = 0; i<this->identifiers.size(); i++){
        const std::string &VarName = identifiers[i]->GetName();
        Value *InitVal = ConstantInt::get(theContext, APSInt(64, 0));

        AllocaInst *Alloca = CreateEntryBlockAlloca(TheFunction, VarName);
        builder.CreateStore(InitVal, Alloca);

        OldBindings.push_back(namedValues[VarName]);

        namedValues[VarName] = Alloca;
    }
    return OldBindings;
}

std::vector<AllocaInst *> VariableDeclarationsAST::DoAllocations(){
    std::vector<AllocaInst *> OldBindings;
    for(auto &Decl : this->declarations){
        auto old = dynamic_cast<DeclarationAST*>(Decl.get())->DoAllocations();
        OldBindings.insert(OldBindings.end(), old.begin(), old.end());
    }
    return OldBindings;
}

std::vector<AllocaInst *> ConstantDeclarationsAST::DoAllocations(){
    std::vector<AllocaInst *> OldBindings;
    Function *TheFunction = builder.GetInsertBlock()->getParent();
    for(auto Decl : this->constants){
        globalConstants.push_back(Decl.name);
        auto InitVal = ConstantInt::get(theContext, APInt(64, Decl.value));
        
        AllocaInst *Alloca = CreateEntryBlockAlloca(TheFunction, Decl.name);
        builder.CreateStore(InitVal, Alloca);
        OldBindings.push_back(namedValues[Decl.name]);
        namedValues[Decl.name] = Alloca;
    }
    return OldBindings;
}

Value* CallExpessionsAst::codegen(){
    Function* CalleeF;

    if(Callee == "writeln"){
        auto constFunc = theModule->getOrInsertFunction("printf", FunctionType::get(IntegerType::getInt32Ty(theContext), PointerType::get(Type::getInt8Ty(theContext), 0), true /* this is var arg func type*/));
        CalleeF = static_cast<Function*>(constFunc);
    }
    else if(Callee == "readln"){
        auto constFunc = theModule->getOrInsertFunction("__isoc99_scanf", FunctionType::get(IntegerType::getInt32Ty(theContext), PointerType::get(Type::getInt8Ty(theContext), 0), true /* this is var arg func type*/));
        CalleeF = static_cast<Function*>(constFunc);
    }
    else if(Callee == "dec" || Callee=="inc"){
        auto var = dynamic_cast<VariableIdentifierAST*>(Args[0].get());
        if(!var)
            return LogErrorV("DEC must be called with a variable identifier");
        
        Value *Variable = namedValues[var->GetName()];
        if(!Variable){
            printf("Unknown variable name %s\n", var->GetName().c_str()); 
            return nullptr;
        }
        Value* StepVal;
        if(Callee == "dec") StepVal = ConstantInt::get(theContext, APInt(64, -1));
        else StepVal = ConstantInt::get(theContext, APInt(64, 1));
        
        Value *CurVar = builder.CreateLoad(Variable, var->GetName().c_str());
        Value *NextVar = builder.CreateAdd(CurVar, StepVal, "nextvar");
        return builder.CreateStore(NextVar, Variable);
    }
    /*
    else if(Callee == "exit"){
        auto constFunc = theModule->getOrInsertFunction("exit", FunctionType::get(Type::getVoidTy(theContext), Type::getInt32Ty(theContext), false));
        CalleeF = static_cast<Function*>(constFunc);
    }*/
    else{
        CalleeF = theModule->getFunction(Callee);
    }
    if(!CalleeF){
        printf("Unknown function referenced: %s\n", Callee.c_str());
        return nullptr;
    }


    std::vector<Value*> ArgsV;

    if(Callee == "writeln"){
        ArgsV.push_back(builder.CreateGlobalStringPtr("%d\n", "strtmp"));
    }
    else if(Callee == "readln"){
        ArgsV.push_back(builder.CreateGlobalStringPtr("%d", "strtmp"));
    }
    /*
    else if(Callee == "exit"){
        ArgsV.push_back(ConstantInt::get(theContext, APInt(32, 0)));
    }*/
    if(Callee == "readln"){
        auto arg = dynamic_cast<VariableIdentifierAST*>(Args[0].get());
        if(!arg){
            printf("Improper call to readln. Expected identifier\n");
            return nullptr;
        }
        Value* v = namedValues[arg->GetName()];
        ArgsV.push_back(v);
        //auto val = Args[i]
    }
    else{
        for(int i = 0; i<Args.size(); i++){
            ArgsV.push_back(Args[i]->codegen());
            if(!ArgsV.back())
                return nullptr;
        }
    }

    if(CalleeF->arg_size() != ArgsV.size() && CalleeF->arg_size() != Args.size()){
        printf("%s: %d wanted %d\n", Callee.c_str(), (int) ArgsV.size(),(int) CalleeF->arg_size());
        return LogErrorV("Incorrect # arguments passed");
    }

    if(CalleeF->getReturnType() == Type::getVoidTy(theContext)){
        return builder.CreateCall(CalleeF, ArgsV);
    }
    else{
        return builder.CreateCall(CalleeF, ArgsV, "calltmp");
    }
}

Value* IfExpressionAST::codegen(){
    Value *CondV = cond->codegen();
    if(!CondV)
        return nullptr;

    CondV = builder.CreateICmpNE(CondV, ConstantInt::get(theContext, APInt(1, 0)), "ifcond");
    Function *theFunction = builder.GetInsertBlock()->getParent();
    BasicBlock *thenBB = BasicBlock::Create(theContext, "then", theFunction);
    BasicBlock *elseBB = BasicBlock::Create(theContext, "else");
    BasicBlock *mergeBB = BasicBlock::Create(theContext, "ifcont");

    builder.CreateCondBr(CondV, thenBB, elseBB);

    builder.SetInsertPoint(thenBB);

    //Im sorry for this...
    bool oldHasBrokeFunction = hasBrokeFromFunctionInBlock;
    bool oldHasBrokeLoop = hasBrokeFromLoopInBlock;

    thenPart->codegen();

    if(!hasBrokeFromLoopInBlock && !hasBrokeFromFunctionInBlock){
        builder.CreateBr(mergeBB);
    }
    hasBrokeFromFunctionInBlock = oldHasBrokeFunction;
    hasBrokeFromLoopInBlock = oldHasBrokeLoop;

    thenBB = builder.GetInsertBlock();

    theFunction->getBasicBlockList().push_back(elseBB);
    builder.SetInsertPoint(elseBB);

    //Value *ThenV = Constant::getNullValue(Type::getInt64Ty(theContext));
    //Value *ElseV = Constant::getNullValue(Type::getInt64Ty(theContext));

    //Im sorry for this again...
    oldHasBrokeFunction = hasBrokeFromFunctionInBlock;
    oldHasBrokeLoop = hasBrokeFromLoopInBlock;

    if(elsePart) elsePart->codegen();

    if(!hasBrokeFromLoopInBlock && !hasBrokeFromFunctionInBlock){
        builder.CreateBr(mergeBB);
    }

    hasBrokeFromFunctionInBlock = oldHasBrokeFunction;
    hasBrokeFromLoopInBlock = oldHasBrokeLoop;

    elseBB = builder.GetInsertBlock();
    
    theFunction->getBasicBlockList().push_back(mergeBB);
    builder.SetInsertPoint(mergeBB);
    //PHINode *PN = builder.CreatePHI(Type::getInt64Ty(theContext), 2, "iftmp");

    //PN->addIncoming(ThenV, thenBB);
    //PN->addIncoming(ElseV, elseBB);

    return Constant::getNullValue(Type::getInt64Ty(theContext));
}

Value* ForExpressionAST::codegen(){
    Value *StartVal = start->codegen();
    if(!StartVal)
        return nullptr;

    Function *TheFunction = builder.GetInsertBlock()->getParent();

    AllocaInst *Alloca = CreateEntryBlockAlloca(TheFunction, loopVarName);

    builder.CreateStore(StartVal, Alloca);

    BasicBlock *LoopBB = BasicBlock::Create(theContext, "loop", TheFunction);

    builder.CreateBr(LoopBB);

    builder.SetInsertPoint(LoopBB);

    AllocaInst *OldVal = namedValues[loopVarName];
    namedValues[loopVarName] = Alloca;


    BasicBlock *AfterBB = BasicBlock::Create(theContext, "afterloop", TheFunction);
    auto oldLastLoopEndBlock = lastLoopEndBlock;
    lastLoopEndBlock = AfterBB;

    if(!body->codegen())
        return nullptr;

    lastLoopEndBlock = oldLastLoopEndBlock;

    Value *StepVal = nullptr;
    /*if(step){
        StepVal = step->codegen();
        if(!StepVal)
            return nullptr;
    }
    else{*/
    if(direction == KW_TO){
        StepVal = ConstantInt::get(theContext, APInt(64, 1));
    }
    else{
        StepVal = ConstantInt::get(theContext, APInt(64, -1));
    }
    //}
    Value *CurVar = builder.CreateLoad(Alloca, loopVarName.c_str());
    Value *NextVar = builder.CreateAdd(CurVar, StepVal, "nextvar");
    builder.CreateStore(NextVar, Alloca);

    Value *EndCond = end->codegen();
    if(!EndCond)
        return nullptr;

    auto EndCondV = builder.CreateICmpNE(EndCond, CurVar, "loopcond");

    builder.CreateCondBr(EndCondV, LoopBB, AfterBB);
    builder.SetInsertPoint(AfterBB);

    if(OldVal)
        namedValues[loopVarName] = OldVal;
    else
        namedValues.erase(loopVarName);

    return Constant::getNullValue(Type::getInt64Ty(theContext));
}

Value* WhileExpressionAST::codegen(){

    Function *TheFunction = builder.GetInsertBlock()->getParent();

    BasicBlock *CondBB = BasicBlock::Create(theContext, "loopCondBB", TheFunction);
    BasicBlock *LoopBB = BasicBlock::Create(theContext, "loop", TheFunction);

    builder.CreateBr(CondBB);

    builder.SetInsertPoint(CondBB);

    Value *StartCond = cond->codegen();
    if(!StartCond)
        return nullptr;

    BasicBlock *AfterBB = BasicBlock::Create(theContext, "afterloop", TheFunction);
    auto StartCondV = builder.CreateICmpNE(StartCond, ConstantInt::get(theContext, APInt(1,0)), "loopcond");
    builder.CreateCondBr(StartCondV, LoopBB, AfterBB);

    builder.SetInsertPoint(LoopBB);
    auto oldLastLoopEndBlock = lastLoopEndBlock;
    lastLoopEndBlock = AfterBB;

    if(!body->codegen())
        return nullptr;

    lastLoopEndBlock = oldLastLoopEndBlock;

    builder.CreateBr(CondBB);

    builder.SetInsertPoint(AfterBB);

    return Constant::getNullValue(Type::getInt64Ty(theContext));
}

Value* PrototypeAST::codegen(){
    std::vector<Type*> Ints(Args.size(), Type::getInt64Ty(theContext));

    FunctionType *FT;
    if(returnType == EOI) FT = FunctionType::get(Type::getVoidTy(theContext), Ints, false);
    else FT = FunctionType::get(Type::getInt64Ty(theContext), Ints, false);

    Function *F = Function::Create(FT, Function::ExternalLinkage, name, theModule.get());

    unsigned Idx = 0;
    for(auto &Arg : F->args()){
        Arg.setName(Args[Idx++].name);
    }

    return F;
}

std::vector<AllocaInst*> FunctionAST::DoAllocations(){
    PrototypeAST *proto = dynamic_cast<PrototypeAST*>(prototype.get());
    Function *theFunction = theModule->getFunction((proto->GetName()));

    if(!theFunction)
        theFunction = static_cast<Function*>(proto->codegen());

    if(!theFunction)
        return {};

    if(!theFunction->empty()){
        printf("Function %s cannot be redefined\n", proto->GetName().c_str());
        return {};
    }

    BasicBlock *BB = BasicBlock::Create(theContext, "entry", theFunction);

    //Create return point
    BasicBlock *RetBlock = BasicBlock::Create(theContext, "return", theFunction);

    builder.SetInsertPoint(BB);

    std::vector<AllocaInst *> OldBindings;
    //namedValues.clear();
    for(auto &Arg : theFunction->args()){
        AllocaInst *Alloca = CreateEntryBlockAlloca(theFunction, Arg.getName());

        builder.CreateStore(&Arg, Alloca);

        OldBindings.push_back(namedValues[Arg.getName()]);
        namedValues[Arg.getName()] = Alloca;
    }
    //Create the return variable
    AllocaInst *FunctionRetVal = CreateEntryBlockAlloca(theFunction, proto->GetName());

    //return ConstantInt::get(theContext, APInt(64, value));
    builder.CreateStore(ConstantInt::get(theContext, APInt(64, 0)), FunctionRetVal);
    OldBindings.push_back(namedValues[proto->GetName()]);
    namedValues[proto->GetName()] = FunctionRetVal;

    builder.SetInsertPoint(RetBlock);
    if(proto->GetReturnType() != EOI){
        auto loadedRetVal = builder.CreateLoad(namedValues[proto->GetName()], proto->GetName());
        builder.CreateRet(loadedRetVal);
    }
    else{
        builder.CreateRetVoid();
    }
    lastFunctionReturnBlock = RetBlock;

    builder.SetInsertPoint(BB);

    //Again... I'm so sorry for this. 
    bool oldHasBrokeLoop = hasBrokeFromLoopInBlock;
    bool oldHasBrokeFunction = hasBrokeFromFunctionInBlock;
    if(Value *BodyVal = body->codegen()){
        if(!hasBrokeFromFunctionInBlock){
            builder.CreateBr(RetBlock);
        }
        hasBrokeFromLoopInBlock = oldHasBrokeLoop;
        hasBrokeFromFunctionInBlock = oldHasBrokeFunction;
        verifyFunction(*theFunction);
        return OldBindings;
    }
    theFunction->eraseFromParent();
    return {};
}
