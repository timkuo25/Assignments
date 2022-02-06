import Header from "../components/Header";
import Body from "../components/Body";
import Footer from "../components/Footer";
import { useState } from "react";

const TodoApp = () => {
    const [createdNum, setCreatedNum] = useState(0);
    const [totalNum, setTotalNum] = useState(0);
    const [doneNum, setdoneNum] = useState(0);
    const [todo, setTodo] = useState([]);
    const [mode, setMode] = useState('All');

    const handleEnter = (newTodo, e) => {
        setCreatedNum(prev => prev + 1);
        setTotalNum(prev => prev + 1);
        setTodo(prev => [...prev, {content: newTodo, id: createdNum, done: false}]);
        e.target.value = '';
    }

    const handleDone = (id) => {
        todo.forEach(item => {
            if (item.id === id){
                if (item.done) setdoneNum(prev => prev - 1);
                else setdoneNum(prev => prev + 1);
            }
        });
        
        setTodo(prev => prev.map(item => (item.id === id) ? {...item, done: !item.done} : item));
    }

    const handleDel = (id) => {
        setTotalNum(prev => prev - 1);
        todo.forEach(item => {
            if (item.id === id){
                if (item.done) setdoneNum(prev => prev - 1);
            }
        });
        setTodo(prev => prev.filter(item => item.id !== id));
    }

    const handleMode = (m) => {
        setMode(prev => m);
    }

    const handleClear = () => {
        setTodo(prev => prev.filter(item => !item.done));
        setTotalNum(prev => prev - doneNum);
        setdoneNum(prev => 0);
    }

    return (
        <>
            <Header text="todos"/>
            <Body
                mode={mode}
                todos={todo}
                handleEnter={handleEnter}
                handleDone={handleDone}
                handleDel={handleDel}
                handleMode={handleMode}
            />
            <Footer 
                totalNum={totalNum}
                doneNum={doneNum}
                handleMode={handleMode}
                handleClear={handleClear}
            />
        </>
    );
}

export default TodoApp;
