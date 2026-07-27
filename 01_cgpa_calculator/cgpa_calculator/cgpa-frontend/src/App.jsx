import { useState } from 'react';
import Dashboard      from './pages/Dashboard';
import Students       from './pages/Students';
import AddStudent     from './pages/AddStudent';
import Search         from './pages/Search';
import BackendStatus  from './components/BackendStatus';
import './App.css';

const PAGES = ['Dashboard', 'Étudiants', 'Ajouter', 'Rechercher'];

export default function App() {
  const [page, setPage] = useState('Dashboard');

  return (
    <div className="app">
      <nav className="navbar">
        <span className="brand">🎓 CGPA Calculator</span>
        <div className="nav-links">
          {PAGES.map(p => (
            <button
              key={p}
              className={`nav-btn ${page === p ? 'active' : ''}`}
              onClick={() => setPage(p)}
            >
              {p}
            </button>
          ))}
        </div>
        <BackendStatus />
      </nav>

      <main className="content">
        {page === 'Dashboard'  && <Dashboard  onNavigate={setPage} />}
        {page === 'Étudiants'  && <Students   onNavigate={setPage} />}
        {page === 'Ajouter'    && <AddStudent onNavigate={setPage} />}
        {page === 'Rechercher' && <Search />}
      </main>
    </div>
  );
}
